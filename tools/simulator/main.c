#include <stdio.h>
#include <string.h>
#include "cpu_model.h"
#include "loader_factory.h"
#include "runner.h"
#include "tracer.h"

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo.stim | arquivo.test | arquivo.mem> [--verbose] [--dump <arquivo.csv>]\n", argv[0]);
        return 1;
    }

    const char *filepath = NULL;
    const char *dump_path = NULL;
    bool verbose = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--verbose") == 0) {
            verbose = true;
        } else if ((strcmp(argv[i], "--dump") == 0 || strcmp(argv[i], "--trace") == 0) && (i + 1 < argc)) {
            dump_path = argv[++i];
        } else if (argv[i][0] != '-') {
            filepath = argv[i];
        }
    }

    if (!filepath) {
        fprintf(stderr, "Erro: arquivo de entrada nao especificado\n");
        return 1;
    }

    stimulus_source_t *source = loader_factory_open(filepath);

    if (!source) {
        fprintf(stderr, "Erro ao abrir arquivo de estimulo: %s\n", filepath);
        return 1;
    }

    tracer_t *tracer = NULL;
    if (dump_path) {
        tracer = tracer_open_csv(dump_path);
        if (!tracer) {
            fprintf(stderr, "Aviso: nao consegui criar arquivo de dump '%s'\n", dump_path);
        } else {
            printf("Gerando dump em: %s\n", dump_path);
        }
    }

    cpu_t cpu = {0};

    cpu_step(&cpu);
    run_simulation(&cpu, source, verbose, tracer);

    if (tracer) {
        tracer->close(tracer);
    }
    source->close(source);

    return 0;
}