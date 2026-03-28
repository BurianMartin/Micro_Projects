#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <endian.h>
#include <stdio.h>
#include <sys/stat.h>
#include <zlib.h>
#include <malloc.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define IMAGE_TYPE_PNG 1
#define IMAGE_TYPE_JPEG 2
#define IMAGE_TYPE_UNDEFINED -1

#define PNG_SIGNATURE_BYTES 8
#define PNG_HEADER_BYTES 29

const int png_signature[] = {137, 80, 78, 71, 13, 10, 26, 10};

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

typedef struct __attribute__((packed)) PNG_Header
{
    uint32_t length;

    uint32_t ihdr_type;

    uint32_t width;
    uint32_t height;

    uint8_t bit_depth;
    uint8_t color_type;

    uint8_t compression;
    uint8_t filter_method;

    uint8_t interlace;
    uint32_t crc;
} PNG_Header;

typedef struct IDAT
{
    uint32_t length;
} IDAT;

bool file_exists(const char *filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0 ? true : false;
}

bool open_file(const char *filename, FILE **file)
{
    (*file) = fopen(filename, "rb");

    if ((*file) == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", filename);
        return false;
    }
    fprintf(stdout, "File sucessfully opened\n");
    return true;
}

int close_file(FILE **file)
{
    if (fclose(*file))
    {
        fprintf(stderr, "Error closing the image file\n");
        return 1;
    }
    fprintf(stdout, "File sucessfully closed, terminating the program\n");
    return 0;
}

uint8_t paeth_predictor(uint8_t a, uint8_t b, uint8_t c)
{
    int p = (int)a + (int)b - (int)c;
    int pa = abs(p - (int)a);
    int pb = abs(p - (int)b);
    int pc = abs(p - (int)c);

    if (pa <= pb && pa <= pc)
        return a;
    else if (pb <= pc)
        return b;
    else
        return c;
}

void load_png_header(FILE **file, PNG_Header *header)
{
    fread(header, sizeof(PNG_Header), 1, *file);

    header->length = be32toh(header->length);
    header->ihdr_type = be32toh(header->ihdr_type);
    header->width = be32toh(header->width);
    header->height = be32toh(header->height);
    header->crc = be32toh(header->crc);
}

int get_file_type(FILE **file)
{

    uint8_t first_byte = 0;
    fread(&first_byte, 1, 1, (*file));

    switch (first_byte)
    {
    case 0xFF:
    {
        uint8_t second_byte = 0;
        fread(&second_byte, 1, 1, (*file));
        if (second_byte == 0xD8)
            return IMAGE_TYPE_JPEG;
    }
    case 0x89:
    {
        uint8_t byte = 0;

        for (int i = 1; i < PNG_SIGNATURE_BYTES; i++)
        {
            fread(&byte, 1, 1, (*file));
            if (byte != png_signature[i])
            {
                return IMAGE_TYPE_UNDEFINED;
            }
        }
        return IMAGE_TYPE_PNG;
        break;
    }
    default:
        return IMAGE_TYPE_UNDEFINED;
        break;
    }
}

bool draw_image(FILE **image)
{
    PNG_Header header;

    load_png_header(image, &header);

    int pixel_count = header.width * header.height;

    fprintf(stdout, "File width: %d, file height: %d, pixels: %d\n", header.width, header.height, pixel_count);

    uint8_t *idat_buffer = NULL;
    size_t idat_size = 0;

    uint8_t r, g, b;
    while (1)
    {
        uint32_t length;
        uint32_t type;

        fread(&length, 4, 1, *image);
        fread(&type, 4, 1, *image);

        length = be32toh(length);
        type = be32toh(type);

        if (type == 0x49454E44) // "IEND"
            break;

        if (type == 0x49444154) // "IDAT"
        {
            idat_buffer = realloc(idat_buffer, idat_size + length);
            fread(idat_buffer + idat_size, length, 1, *image);
            idat_size += length;
        }
        else
        {
            fseek(*image, length, SEEK_CUR);
        }

        fseek(*image, 4, SEEK_CUR);
    }

    size_t bytes_per_pixel = 3;
    size_t output_size = header.height * (1 + header.width * bytes_per_pixel);
    uint8_t *decompressed = malloc(output_size);

    z_stream stream = {0};
    inflateInit(&stream);
    stream.next_in = idat_buffer;
    stream.avail_in = idat_size;
    stream.next_out = decompressed;
    stream.avail_out = output_size;
    inflate(&stream, Z_FINISH);
    inflateEnd(&stream);

    free(idat_buffer);

    size_t row_size = 1 + header.width * bytes_per_pixel;
    uint8_t *pixels = malloc(pixel_count * bytes_per_pixel);

    for (uint32_t y = 0; y < header.height; y++)
    {
        uint8_t filter = decompressed[y * row_size];
        uint8_t *row = decompressed + y * row_size + 1;
        uint8_t *out = pixels + y * header.width * bytes_per_pixel;
        uint8_t *above = (y == 0) ? NULL : pixels + (y - 1) * header.width * bytes_per_pixel;

        for (uint32_t x = 0; x < header.width * bytes_per_pixel; x++)
        {
            uint8_t left = (x < bytes_per_pixel) ? 0 : out[x - bytes_per_pixel];
            uint8_t up = above ? above[x] : 0;

            switch (filter)
            {
            case 0:
                out[x] = row[x];
                break;
            case 1:
                out[x] = row[x] + left;
                break;
            case 2:
                out[x] = row[x] + up;
                break;
            case 3:
                out[x] = row[x] + (left + up) / 2;
                break;
            case 4:
            {
                uint8_t up_left = (above && x >= bytes_per_pixel) ? above[x - bytes_per_pixel] : 0;
                out[x] = row[x] + paeth_predictor(left, up, up_left);
                break;
            }
            }
        }
    }

    free(decompressed);

    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STATIC,
        header.width,
        header.height);

    SDL_UpdateTexture(texture, NULL, pixels, header.width * bytes_per_pixel);

    free(pixels);

    // render it
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);

    return true;
}

int main(int argc, char const *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Incorrect program usage. Correct usage example:\n\t./Image_Viewer image.png\n");
        return 1;
    }
    else if (!file_exists(argv[1]))
    {
        fprintf(stderr, "The file %s does not seem to exist\n", argv[1]);
        return 1;
    }

    FILE *image;
    if (!open_file(argv[1], &image))
    {
        return 1;
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL init error: %s\n", SDL_GetError());
        return 1;
    }
    if (!SDL_CreateWindowAndRenderer("Image Viewer", 1800, 1200, 0, &window, &renderer))
    {
        fprintf(stderr, "SDL window error: %s\n", SDL_GetError());
        return 1;
    }

    switch (get_file_type(&image))
    {
    case IMAGE_TYPE_JPEG:
    {
        fprintf(stdout, "Your file type is JPG/JPEG (They are the same thing)\n");
        break;
    }

    case IMAGE_TYPE_PNG:
    {
        fprintf(stdout, "Your file type is PNG\n");

        draw_image(&image);

        break;
    }

    case IMAGE_TYPE_UNDEFINED:
    {
        fprintf(stderr, "Unknown file type. We accept only PNG and JPG/JPEG for now :(\n");
        break;
    }

    default:
    {
        fprintf(stderr, "IDK what the hell you did, but you broke me :(\n");
        break;
    }
    }

    close_file(&image);

    SDL_Event event;
    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_EVENT_QUIT)
                running = false;
    }

    return 0;
}
