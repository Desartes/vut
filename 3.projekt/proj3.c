/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/slsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;
    c->capacity = cap;

    if ( !(c->obj = malloc(cap*sizeof(struct obj_t))) )
        puts("init_cluster not succesful.");
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO

    free(c->obj);
    c->size = 0;
    c->capacity = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;
    
/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */ 
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO

    if (c->size < c->capacity) {
        c->obj[c->size++] = obj;
    } else {
        if (resize_cluster(c, c->capacity + 1) != NULL) {
            c->obj[c->size++] = obj;
        } else {
            puts("Fail.");
        }
    }
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO
    for (int i = 0; i < c2->size; ++i) {
        append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    clear_cluster(&carr[idx]);

    for (int i = idx; i < narr -1; ++i) {
        carr[i] = carr[i + 1]; 
    }
    return --narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO
    float result,
          x1 = o1->x, y1 = o1->y,
          x2 = o2->x, y2 = o2->y;
    
    result = sqrtf( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
    return result;
}

/*
 Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
 souseda.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    // TODO
    float result = 10000,
          temp   = 0;
    for (int i = 0; i < c1->size; ++i) {
        for (int n = 0; n < c2->size; ++n) {
            if ( (temp = obj_distance(&(c1->obj[i]), &(c2->obj[n]))) < result ) {
                result = temp;
            }
        }
    }
    return result;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    // TODO
    float result = cluster_distance(carr, carr + 1),
          temp;
    int st,nd;
    for (int i = 0; i < narr; ++i) {
        for (int n = i + 1; n < narr; ++n) {
            if ( (temp = cluster_distance(carr + i, carr + n)) < result ) {
                result = temp;
                st = i;
                nd = n;
            }
        }
    }
    *c1 = st;
    *c2 = nd;
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{       
    assert(arr != NULL);

    FILE *file;
    struct obj_t object;

    if ( (file = fopen(filename, "r")) ) {
        char buffer[20];
        if ( fscanf(file, "%s", buffer) ) {
            int count;
            sscanf(buffer, "count=%d", &count);
            *arr = malloc(count*sizeof(struct cluster_t)); 

            for (int i = 0; i < count; i++) {
                struct cluster_t cluster;
                fscanf(file, "%d %f %f", &object.id, &object.x, &object.y);
                init_cluster(&cluster, CLUSTER_CHUNK);
                append_cluster(&cluster, object);
                (*arr)[i] = cluster;
            }
            fclose(file);
            return count;
        } else {
            puts("Error: Bad format of data file.");
            return -1;
        }
        fclose(file);
        
    } else {
        puts("Error: Couldn't load file.");
        return -2;
    }
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);

    }
}
int main(int argc, char *argv[])
{
    (void) argc;
    struct cluster_t *clusters;
    int count, ne1=0,ne2=0;
    count = load_clusters(argv[1], &clusters);
    if (!argv[2]) {
        while ( clusters[1].size != 0 ) {
            merge_clusters(&clusters[0], &clusters[1]);
            count = remove_cluster(clusters, count, 1);
        }
    } else {
        if (atoll(argv[2]) <= INT_MAX && atoll(argv[2]) > 0) {
            int clust = atoi(argv[2]);
            int s_count = count - clust;
            if (clust == 1) {
                while ( clusters[1].size != 0 ) {
                    merge_clusters(&clusters[0], &clusters[1]);
                    count = remove_cluster(clusters, count, 1);
                }
            } else {
                for (int i = 0; i < s_count; ++i) {
                        find_neighbours(clusters, count,&ne1,&ne2);
                        merge_clusters(&clusters[ne1], &clusters[ne2]);
                        count = remove_cluster(clusters, count, ne2);
                    
                }            
            }
        } else {
            if ( !atoll(argv[2]) ) {
                puts("Error: Third argument has to be number.");
            } else {
                puts("Error: Number in third argument is out of range.");
            }
            return 1;
        }
    }
    print_clusters(clusters, count);
    while (count > 0)
        count = remove_cluster(clusters, count, (count -1));
    free(clusters);
    return 0;

}