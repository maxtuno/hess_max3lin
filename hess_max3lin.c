/*
///////////////////////////////////////////////////////////////////////////////
//        Copyright (c) 2012-2020 Oscar Riveros. all rights reserved.        //
//                        oscar.riveros@peqnp.science                        //
//                                                                           //
//   without any restriction, Oscar Riveros reserved rights, patents and     //
//  commercialization of this knowledge or derived directly from this work.  //
///////////////////////////////////////////////////////////////////////////////

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

int n;
int m;
int *sizes;
int **cls;
int cursor;

struct node {
    int *model;
    int *temporal;
    int *optimal;
};

int oracle(struct node *node) {
    int i, j, s, c = 0;
    for (i = 0; i < m; i++) {
        s = 0;
        for (j = 0; j < sizes[i] - 1; j++) {
            s += node->model[abs(cls[i][j]) - 1] == cls[i][j] > 0;
        }
        c += s % 2 == abs(cls[i][sizes[i] - 1]);
    }
    return c;
}

void step(int i, int j, struct node *node) {
    int aux;
    if (node->model[i] == node->model[j]) {
        node->model[i] = !node->model[j];
    } else {
        aux = node->model[i];
        node->model[i] = !node->model[j];
        node->model[j] = aux;
    }
}

void hess(struct node *node) {
    int i, j, k, loc, glb;
    do {
        glb = 0;
        o:
        for (i = 0; i < n; i++) {
            oo:
            for (j = 0; j < n; j++) {
                ooo:
                step(i, j, node);
                loc = oracle(node);
                if (loc > glb) {
                    glb = loc;
                    if (glb > cursor) {
                        cursor = glb;
#ifdef LOG
                        printf("\rc REMAINING : %3.2lf%% ", 100.0 - 100.0 * cursor / m);
                        fflush(stdout);
#endif

                        memcpy(node->optimal, node->model, sizeof(int) * n);
                        if (m == cursor) {
                            return;
                        }
                        goto o;
                    }
                    goto oo;
                } else if (loc < glb) {
                    goto ooo;
                }
            }
        }
        for (i = 0; i < n; i++) {
            node->model[i] = !node->model[i];
        }
    } while (glb > cursor);
}

void apply() {
    int i, j;
    struct node node;

    node.temporal = (int *) calloc((size_t) n, sizeof(int));
    node.model = (int *) calloc((size_t) n, sizeof(int));
    node.optimal = (int *) calloc((size_t) n, sizeof(int));

    for (i = 0; i < n; i++) {
        node.model[i] = i % 2;
    }

    hess(&node);

    printf("\nc SATISFIED : %i vs %i\nc BREAK INAPPROXIMABILITY RATIO?, %s", m - cursor, m / 2, (m - cursor < m / 2 ? "YES" : "NO"));
    for (i = 0; i < n; i++) {
        if (i % 10 == 0) {
            printf("\nv ");
        }
        printf("%i ", node.optimal[i] ? +(i + 1) : -(i + 1));
    }
    printf("0\n");

    free(node.temporal);
    free(node.model);
    free(node.optimal);
}

int main(int argc, char **argv) {
    int i, j;
    char buffer[32];

    if (argc != 2) {
        printf("c /////////////////////////////////////////////////////////////////////////////\n");
        printf("c //       Copyright (c) 2012-2020 Oscar Riveros. all rights reserved.       //\n");
        printf("c //                       oscar.riveros@peqnp.science                       //\n");
        printf("c //                                                                         //\n");
        printf("c //  without any restriction, Oscar Riveros reserved rights, patents and    //\n");
        printf("c // commercialization of this knowledge or derived directly from this work. //\n");
        printf("c /////////////////////////////////////////////////////////////////////////////\n");
        printf("c                                                                              \n");
        printf("c usage: %s <instance>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("c               \n");
    printf("c ╦ ╦╔═╗╔═╗╔═╗  \n");
    printf("c ╠═╣║╣ ╚═╗╚═╗  \n");
    printf("c ╩ ╩╚═╝╚═╝╚═╝  \n");
    printf("c www.peqnp.com \n");
    printf("c                  \n");
    printf("c INSTANCE  : %s\n", argv[1]);

    FILE *file = fopen(argv[1], "r");
    if (strcmp(buffer, "c") == 0) {
        while (strcmp(buffer, "\n") != 0) {
            fscanf(file, "%s", buffer);
        }
    }
    while (strcmp(buffer, "p") != 0) {
        fscanf(file, "%s", buffer);
    }
    fscanf(file, " lin %i %i", &n, &m);
    sizes = (int *) calloc((size_t) m, sizeof(int));
    cls = (int **) calloc((size_t) m, sizeof(int *));

    printf("c VARIABLES : %d\n", n);
    printf("c EQUATIONS : %d\n", m);
    printf("c RATIO     : %lf\n", (double) m / n);

    for (i = 0; i < m; i++) {
#ifdef LOG
        printf("\rc LOADING   : %3.2lf%%", 100.0 * (i + 1) / m);
        fflush(stdout);
#endif
        j = 0;
        cls[i] = (int *) calloc((size_t) n + 1, sizeof(int));
        do {
            fscanf(file, "%s", buffer);
            if (strcmp(buffer, "c") == 0) {
                continue;
            }
            cls[i][j++] = atoi(buffer);
            if (abs(cls[i][j - 1]) == n + 1) {
                cls[i][j - 1] = cls[i][j - 1] > 0;
            }
        } while (strcmp(buffer, "0") != 0);
        j--;
        cls[i] = (int *) realloc(cls[i], j * sizeof(int));
        sizes[i] = j;
    }
    fclose(file);

    cursor = 0;

    apply();

    free(sizes);
    free(cls);

    return EXIT_SUCCESS;
}
