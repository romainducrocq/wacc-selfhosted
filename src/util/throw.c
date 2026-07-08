#include "c_lib.h"

#include "util/c_std.h"
#include "util/fileio.h"
#include "util/throw.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Throw

#define Ctx struct ErrorsContext*

#define ESC 27
static char esc_reset[5] = { ESC, '[', '0', 'm', 0 };
static char esc_bold[5] = { ESC, '[', '1', 'm', 0 };
static char esc_red[8] = { ESC, '[', '0', ';', '3', '1', 'm', 0 };

void panic_sigabrt(char* msg, int line, char* file) {
    fflush(stdout);
    {
        string_t strto_line = str_to_string(line);
        fprintf(stderr, "%*s%*s:%*s:%*s\n%*s", 0, esc_bold, 0, file, 0, strto_line, 0, esc_reset, 0, "");
        fprintf(stderr, "%*sinternal error:%*s %*s\n%*s%*s", 0, esc_red, 0, esc_reset, 0, msg, 0, "", 0, "");
        str_delete(strto_line);
    }
    abort();
}

void raise_init_error(Ctx ctx) {
    if (ctx->is_stdout) {
        printf("%s", "\n");
        fflush(stdout);
    }
    fprintf(stderr, "%*serror:%*s %*s\n%*s%*s", 0, esc_red, 0, esc_reset, 0, ctx->msg, 0, "", 0, "");
}

void raise_base_error(Ctx ctx) {
    free_fileio(ctx->fileio);
    char* filename = get_filename(ctx->fileio);
    if (ctx->is_stdout) {
        printf("%s", "\n");
        fflush(stdout);
    }
    fprintf(stderr, "%*s%*s:%*s\n%*s%*s", 0, esc_bold, 0, filename, 0, esc_reset, 0, "", 0, "");
    fprintf(stderr, "%*serror:%*s %*s\n%*s%*s", 0, esc_red, 0, esc_reset, 0, ctx->msg, 0, "", 0, "");
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
        string_t strto_pos = str_to_string(tok_pos);

        fprintf(stderr, "%*s%*s:%*s:%*s:%*s\n", 0, esc_bold, 0, filename, 0, strto_linenum, 0, strto_pos, 0, esc_reset);
        fprintf(stderr, "%*serror:%*s %*s\n%*s%*s", 0, esc_red, 0, esc_reset, 0, ctx->msg, 0, "", 0, "");
        fprintf(stderr, "at line %*s: %*s%*sv%*s%*s\n", 0, strto_linenum, 0, esc_red, pad_tok, "", 0, tok_overline, 0, esc_reset);
        fprintf(stderr, "        %*s| %*s%*s%*s\n%*s", pad_linenum, "", 0, esc_bold, 0, line, 0, esc_reset, 0, "");

        str_delete(tok_overline);
        str_delete(strto_linenum);
        str_delete(strto_pos);
    }
    str_delete(line);
}
