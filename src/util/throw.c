#include "c_lib.h"

#include "util/c_std.h"
#include "util/fileio.h"
#include "util/throw.h"

// TODO remove
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"
extern int fprintf(struct FILE* stream, char* format, ...);
#pragma GCC diagnostic pop

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Throw

#define Ctx struct ErrorsContext*

void panic_sigabrt(char* msg, char* func, int line, char* file) {
    fflush(stdout);
    fprintf(stderr,
        "\033[1m%s:%i:\033[0m\n"
        "\033[0;31minternal error:\033[0m %s (%s)\n",
        file, line, func, msg);
    abort();
}

void raise_init_error(Ctx ctx) {
    if (ctx->is_stdout) {
        printf("%s", "\n");
        fflush(stdout);
    }
    fprintf(stderr, "\033[0;31merror:\033[0m %s\n", ctx->msg);
}

void raise_base_error(Ctx ctx) {
    free_fileio(ctx->fileio);
    char* filename = get_filename(ctx->fileio);
    if (ctx->is_stdout) {
        printf("%s", "\n");
        fflush(stdout);
    }
    fprintf(stderr,
        "\033[1m%s:\033[0m\n"
        "\033[0;31merror:\033[0m %s\n",
        filename, ctx->msg);
}

static unsigned long get_token_linenum(Ctx ctx, unsigned long total_linenum) {
    for (unsigned long i = 0; i < vec_size(ctx->fopen_lines) - 1; ++i) {
        if (total_linenum < ctx->fopen_lines[i + 1].total_linenum) {
            set_filename(ctx->fileio, ctx->fopen_lines[i].filename);
            return total_linenum - ctx->fopen_lines[i].total_linenum + ctx->fopen_lines[i].linenum;
        }
    }
    set_filename(ctx->fileio, vec_back(ctx->fopen_lines).filename);
    return total_linenum - vec_back(ctx->fopen_lines).total_linenum + vec_back(ctx->fopen_lines).linenum;
}

void raise_error_at_token(Ctx ctx, unsigned long info_at) {
    THROW_ABORT_IF(info_at >= vec_size(ctx->errors->token_infos));
    struct TokenInfo* token_info = &ctx->errors->token_infos[info_at];
    unsigned long tok_linenum = get_token_linenum(ctx, token_info->total_linenum);

    free_fileio(ctx->fileio);
    char* filename = get_filename(ctx->fileio);
    string_t line = str_new(NULL);
    {
        unsigned long len = 0;
        char* buf = NULL;
        struct FILE* fd = fopen(filename, "rb");
        if (!fd) {
            raise_base_error(ctx);
            return;
        }
        for (unsigned long i = 0; i < tok_linenum; ++i) {
            if (getline(&buf, &len, fd) == -1) {
                free(buf);
                fclose(fd);
                buf = NULL;
                fd = NULL;
                raise_base_error(ctx);
                return;
            }
        }
        line = str_new(buf);
        free(buf);
        fclose(fd);
        buf = NULL;
        fd = NULL;
        if (str_back(line) == '\n') {
            str_pop_back(line);
        }
    }
    if (ctx->is_stdout) {
        printf("%s", "\n");
        fflush(stdout);
    }
    {
        string_t tok_overline = str_new("");
        string_t strto_linenum = str_to_string(tok_linenum);

        int tok_pos = 1;
        if (token_info->tok_pos >= 0) {
            tok_pos += token_info->tok_pos;
            if (token_info->tok_len > 1) {
                str_resize(tok_overline, token_info->tok_len - 1);
                for (unsigned long i = 0; i < str_size(tok_overline); ++i) {
                    tok_overline[i] = '~';
                }
            }
        }
        int pad_tok = tok_pos - 1;
        int pad_linenum = (int)str_size(strto_linenum);
        if (pad_linenum < 0) {
            pad_linenum = 0;
        }

        fprintf(stderr,
            "\033[1m%s:%zu:%i:\033[0m\n"
            "\033[0;31merror:\033[0m %s\n"
            "at line %s: \033[0;31m%*sv%s\033[0m\n"
            "        %*s| \033[1m%s\033[0m\n",
            filename, tok_linenum, tok_pos, ctx->msg, strto_linenum, pad_tok, "", tok_overline, pad_linenum, "", line);

        str_delete(tok_overline);
        str_delete(strto_linenum);
    }
    str_delete(line);
}
