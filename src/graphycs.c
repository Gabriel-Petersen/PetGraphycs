#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <conio.h>
#include <ctype.h>
#include <limits.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "graphycs.h"

/**
 * @internal
 */
typedef struct Obj_Node {
    void** obj_adress;
    char obj_type;
    struct Obj_Node* anterior;
} Obj_Node;
/**
 * @internal
 */
typedef struct {
    Obj_Node* topo;
} Obj_Stack;
/**
 * @internal
 */
static Obj_Stack* criar_obj_stack()
{
    Obj_Stack* stack = (Obj_Stack*)malloc(sizeof(Obj_Stack));
    stack->topo = NULL;
    return stack;
}
/**
 * @internal
 */
Obj_Node pop_obj(Obj_Stack* stack)
{
    Obj_Node topo = (Obj_Node){NULL, 'n', NULL};
    if (stack->topo == NULL) return topo;
    Obj_Node* x = stack->topo;
    stack->topo = x->anterior;
    topo = *x;
    free(x);
    return topo;
}
/**
 * @internal
 */
void push_Obj(Obj_Stack* stack, void **obj, char obj_type) 
{
    Obj_Node* atual = stack->topo;
    while (atual != NULL) 
    {
        if (atual->obj_adress == obj) 
            return;
        atual = atual->anterior;
    }
    
    Obj_Node* novo = (Obj_Node*)malloc(sizeof(Obj_Node));

    novo->obj_adress = obj;
    novo->obj_type = obj_type;
    novo->anterior = stack->topo;
    stack->topo = novo;
}

char ler_teclado()
{
    char ultima = '\0';
    while (_kbhit())
        ultima = toupper(getch());

    return ultima;
}

void print_vector(Vector2 v, char* name)
{
    printf("%s: (%d, %d)\n", name, v.x, v.y);
}

/**
 * @internal
 */
int next_anim(Animation* anim)
{
    return (anim->frame_atual + 1 == anim->qtd_frames) ? 0 : anim->frame_atual + 1;
}

/**
 * @internal
 */
Color converter_ABGR_para_Color(uint32_t abgr) 
{
    uint8_t alpha = (abgr >> 24) & 0xFF;
    if(alpha == 0)
        return COLOR_PRETO;
    
    uint8_t r = abgr & 0xFF; // Obtém o vermelho
    uint8_t g = (abgr >> 8)  & 0xFF; // Obtém o verde
    uint8_t b = (abgr >> 16) & 0xFF; // Obtém o azul

    return criar_cor(r, g, b);
}

Color criar_cor(int r, int g, int b) {
    return (Color){r, g, b};
}

#define min(a,b) (a < b ? a : b)
#define max(a,b) (a > b ? a : b)

Color aplicar_filtro (Color original, Color filtro)
{
    return criar_cor(
        min (255, original.r * filtro.r / 255),
        min (255, original.g * filtro.g / 255),
        min (255, original.b * filtro.b / 255)
    );
}

Vector2 new_Vector2 (int x, int y)
{
    return (Vector2){x, y};
}

Vector2 produto_vetor_escalar (Vector2 vet, float escalar)
{
    return new_Vector2(
        vet.x * (int)round(escalar),
        vet.y * (int)round(escalar)
    );
}

Vector2 vector_sum (Vector2 v1, Vector2 v2) {
    return new_Vector2 (
        v1.x + v2.x, 
        v1.y + v2.y
    );
}

Vector2 vector_subtr(Vector2 v1, Vector2 v2)
{
    return new_Vector2 (
        v1.x - v2.x, 
        v1.y - v2.y
    ); 
}

Vector2 reverse_vector (Vector2 v){
    return new_Vector2 (-v.x, -v.y);
}

Vector2 centro_objeto_complexo(ObjetoComplexo* obj)
{
    return produto_vetor_escalar(obj->size, 0.5f);
}

Vector2 get_complexo_abs_Pixel_pos(ObjetoComplexo* obj, int frame, int index)
{
    return new_Vector2(
        obj->frames[frame]->info[index].position.x + obj->position.x,
        obj->frames[frame]->info[index].position.y + obj->position.y
    );
}

//INTERNA
Vector2 aplicar_matriz_rot (Vector2 Pixel_pos, Vector2 rot_pivo, float graus)
{
    float radianos = graus * M_PI/180.0;
    float x_rel = Pixel_pos.x - rot_pivo.x;
    float y_rel = -(Pixel_pos.y - rot_pivo.y);

    float x_rotacionado = x_rel * cos(radianos) - y_rel * sin(radianos);
    float y_rotacionado = x_rel * sin(radianos) + y_rel * cos(radianos);

    return new_Vector2(round(x_rotacionado + rot_pivo.x), round(-y_rotacionado + rot_pivo.y));
}

Pixel criar_Pixel (Color cor, Vector2 pos)
{
    Pixel p;
    p.cor = cor;
    p.position = pos;
    p.obj_source = NULL;
    p.source_type = 'n';
    return p;
}

static Pixel_Stack* criar_pilha()
{
    Pixel_Stack* stack = (Pixel_Stack*)malloc(sizeof(Pixel_Stack));
    stack->topo = NULL;
    return stack;
}

static Pixel_Node* criar_nodo (Pixel p)
{
    Pixel_Node* n = (Pixel_Node*)malloc(sizeof(Pixel_Node));
    if (n) 
    {
        n->pixel = p;
        n->anterior = NULL;
    }
    return n;
}

void add_Pixel (Pixel_Stack* stack, Pixel p)
{
    Pixel_Node* x = criar_nodo(p);
    x->anterior = stack->topo;
    stack->topo = x;
}

void limpar_pilha(Pixel_Stack* pixel_stack, Obj_Stack* obj_stack)
{
    if (pixel_stack == NULL || pixel_stack->topo == NULL)
        return;

    Pixel_Node* atual = pixel_stack->topo;
    Pixel ultimo_px = atual->pixel;
    Pixel_Node* anterior;

    while (atual != NULL) 
    {
        if (atual->pixel.obj_source)
            push_Obj(obj_stack, atual->pixel.obj_source, atual->pixel.source_type);
        
        ultimo_px = atual->pixel;
        anterior = atual->anterior;
        free(atual);
        atual = anterior;
    }

    pixel_stack->topo = NULL;
    add_Pixel(pixel_stack, ultimo_px);
}

void mover_tela (Screen* s, Vector2 direction)
{
    s->position = vector_sum(s->position, direction);
    Obj_Stack* pilha = criar_obj_stack();

    for (int i = 0; i < s->screen_size.y; i++) for (int j = 0; j < s->screen_size.x; j++)
        limpar_pilha(s->pixeis[i][j], pilha);
    
    while (pilha->topo != NULL)
    {
        Obj_Node node = pop_obj(pilha);

        if (node.obj_type == 'o') 
        {
            Objeto* o = (Objeto*)node.obj_adress;
            o->renderizado = false;
            desenhar_objeto(s, o);
        }
        else if (node.obj_type == 'c') 
        {
            ObjetoComplexo* oc = (ObjetoComplexo*)node.obj_adress;
            oc->renderizado = false;
            desenhar_objeto_complexo(s, oc);
        }
    }
    free(pilha);
}

Vector2 get_abs_Pixel_pos (Objeto* obj, int index)
{
    return vector_sum(obj->info[index].position, obj->position);
}

Vector2 centro_do_objeto(Objeto *obj) 
{
    return produto_vetor_escalar(obj->size, 0.5f);
}

Vector2 centro_da_tela(Screen* s)
{
    return new_Vector2(
        s->screen_size.x / 2,
        s->screen_size.y / 2
    );
}

Pixel desempilhar_Pixel (Pixel_Stack* stack)
{
    Pixel_Node* x = stack->topo;
    Pixel noPixel = criar_Pixel(COLOR_PRETO, VETOR_NULO);
    if (x == NULL) return noPixel;
    stack->topo = stack->topo->anterior;
    noPixel = x->pixel;
    free(x);
    return noPixel;
}

void normalizar_objeto(Objeto* obj) 
{
    if (obj->qtd_Pixel <= 0)
        return;
    
    int min_x = obj->info[0].position.x;
    int min_y = obj->info[0].position.y;
    
    for (int i = 1; i < obj->qtd_Pixel; i++) 
    {
        if (obj->info[i].position.x < min_x) min_x = obj->info[i].position.x;
        if (obj->info[i].position.y < min_y) min_y = obj->info[i].position.y;
    }
    
    for (int i = 0; i < obj->qtd_Pixel; i++) 
    {
        obj->info[i].position.x -= min_x;
        obj->info[i].position.y -= min_y;
    }
}

void centralizar_objeto(Objeto* obj)
{
    if (obj->qtd_Pixel <= 0)
        return;

    Vector2 min = new_Vector2(INT_MAX, INT_MAX);
    Vector2 max = new_Vector2(INT_MIN, INT_MIN);

    for (int i = 0; i < obj->qtd_Pixel; i++) {
        Vector2 p = obj->info[i].position;
        if (p.x < min.x) min.x = p.x;
        if (p.y < min.y) min.y = p.y;
        if (p.x > max.x) max.x = p.x;
        if (p.y > max.y) max.y = p.y;
    }

    Vector2 centro = new_Vector2(
        (min.x + max.x) / 2,
        (min.y + max.y) / 2
    );

    for (int i = 0; i < obj->qtd_Pixel; i++) 
        obj->info[i].position = vector_subtr(obj->info[i].position, centro);

    obj->size = vector_subtr(max, min);
}

Objeto* criar_objeto_custom (Pixel* info, int qtd_Pixel, bool normalizar)
{
    Objeto* o = (Objeto*)malloc(sizeof(Objeto));
    o->info = info;
    o->position = VETOR_NULO;
    o->qtd_Pixel = qtd_Pixel;
    o->renderizado = false;
    Vector2 menor = info[0].position;
    Vector2 maior = VETOR_NULO;
    for (int i = 0; i < qtd_Pixel; i++)
    {
        if (info[i].position.x < menor.x)
            menor.x = info[i].position.x;
        if (info[i].position.y < menor.y)
            menor.y = info[i].position.y;
        if (info[i].position.x > maior.x)
            maior.x = info[i].position.x;
        if (info[i].position.y > maior.y)
            maior.y = info[i].position.y;
    }

    Vector2 tamanho = new_Vector2(maior.x - menor.x, maior.y - menor.y);
    o->size = tamanho;
    if (normalizar)
        normalizar_objeto(o);
    for (int i = 0; i < qtd_Pixel; i++)
    {
        info[i].obj_source = (void**)o;
        info[i].source_type = 'o';
    }
    return o;
}

Objeto* criar_retangulo_monocromatico (Color cor, Vector2 tamanho)
{
    Objeto* o = (Objeto*)malloc(sizeof(Objeto));
    int area = tamanho.x * tamanho.y;
    o->qtd_Pixel = area;
    o->position = VETOR_NULO;
    o->renderizado = false;
    o->size = tamanho;
    Pixel* info = (Pixel*)malloc(area*sizeof(Pixel));
    for (int i = 0; i < area; i++)
    {
        info[i].cor = cor;
        info[i].position = new_Vector2(i % tamanho.x, i / tamanho.x);
    }
    o->info = info;
    for (int i = 0; i < area; i++)
    {
        info[i].obj_source = (void**)o;
        info[i].source_type = 'o';
    }
    return o;
}

Objeto* recortar_objeto(Objeto* obj, Vector2 inicio, Vector2 fim)
{
    Vector2 min_v = inicio;
    Vector2 max_v = fim;

    min_v = new_Vector2(min(inicio.x, fim.x), min(inicio.y, fim.y));
    max_v = new_Vector2(max(inicio.x, fim.x), max(inicio.y, fim.y));

    inicio = min_v;
    fim = max_v;

    int nova_qtd_Pixel = 0;
    Pixel* novos_pixeis = (Pixel*)malloc(obj->qtd_Pixel * sizeof(Pixel));
    for (int i = 0; i < obj->qtd_Pixel; i++)
    {
        Vector2 Pixel_pos = obj->info[i].position;
        if (Pixel_pos.x >= inicio.x && Pixel_pos.x <= fim.x && Pixel_pos.y >= inicio.y && Pixel_pos.y <= fim.y)
            novos_pixeis[nova_qtd_Pixel++] = obj->info[i];
    }
    novos_pixeis = realloc(novos_pixeis, nova_qtd_Pixel*sizeof(Pixel));
    return criar_objeto_custom(novos_pixeis, nova_qtd_Pixel, true);
}

Objeto* criar_obj_eixos_debug(Screen* s)
{
    int w = s->screen_size.x;
    int h = s->screen_size.y;
    int total = w + h - 1;
    
    Pixel* info = malloc(total * sizeof(*info));
    if (!info) { perror("malloc"); exit(1); }

    int idx = 0;
    int halfW = w/2;
    for (int dx = -halfW; dx < w - halfW; dx++) 
        info[idx++] = criar_Pixel(COLOR_BRANCO, new_Vector2(dx, 0));
    
    int halfH = h/2;
    for (int dy = -halfH; dy < h - halfH; dy++)
        if (dy != 0)
            info[idx++] = criar_Pixel(COLOR_BRANCO, new_Vector2(0, dy));
    
    if (idx != total) {
        fprintf(stderr, "Debug-axes: esperado %d pixels mas gerou %d\n",
                total, idx);
        exit(1);
    }

    return criar_objeto_custom(info, total, false);
}

Objeto* merge_objeto (Objeto* prioridade, Objeto* novo, Vector2 pivot)
{
    int new_size = prioridade->qtd_Pixel;
    Pixel* info_prioridade = (Pixel*)malloc(new_size * sizeof(Pixel));
    memcpy(info_prioridade, prioridade->info, new_size*sizeof(Pixel));
    Pixel* info_novo = novo->info;
    for (int i = 0; i < novo->qtd_Pixel; i++)
        if (!obj_contem_Pixel_em(prioridade, vector_sum(pivot, info_novo[i].position)))
            new_size++;
    
    if (new_size == prioridade->qtd_Pixel) 
    {
        free(info_prioridade);
        excluir_objeto(novo);
        return prioridade;
    }

    info_prioridade = realloc(info_prioridade, new_size * sizeof(Pixel));
    int index = prioridade->qtd_Pixel;
    for (int i = 0; i < novo->qtd_Pixel; i++)
        if (!obj_contem_Pixel_em(prioridade, vector_sum(pivot, info_novo[i].position)))
            info_prioridade[index++] = criar_Pixel(info_novo[i].cor, vector_sum(pivot, info_novo[i].position));
    
    excluir_objeto(prioridade);
    excluir_objeto(novo);
    return criar_objeto_custom(info_prioridade, new_size, true);
}

Pixel deletar_Pixel (Screen* s, Vector2 pos) 
{
    if (!vetor_valido_na_tela(s, pos)) 
        return criar_Pixel(COLOR_PRETO, VETOR_NULO);
    
    return desempilhar_Pixel(s->pixeis[pos.y][pos.x]);
}

Screen* criar_tela (Vector2 tamanho, Color fundo, int limiar_de_cor)
{
    Screen* s = (Screen*)malloc(sizeof(Screen));
    s->screen_size = tamanho;
    s->position = VETOR_NULO;
    s->pixeis = (Pixel_Stack***)malloc(tamanho.y * sizeof(Pixel_Stack**));
    s->buffer = (Color**)malloc(tamanho.y * sizeof(Color*));
    s->limiar_de_cor = limiar_de_cor;
    for (int i = 0; i < tamanho.y; i++)
    {
        s->pixeis[i] = (Pixel_Stack**)malloc(tamanho.x * sizeof(Pixel_Stack*));
        s->buffer[i] = (Color*)malloc(tamanho.x * sizeof(Color));
        for (int j = 0; j < tamanho.x; j++)
        {
            s->pixeis[i][j] = criar_pilha();
            s->buffer[i][j] = COR_NULA;
        }
    }
    preencher_background(s, fundo);
    return s;
}

void excluir_tela(Screen* s) 
{
    if (s == NULL) return;

    if (s->pixeis != NULL) 
    {
        for (int i = 0; i < s->screen_size.y; i++) 
        {
            if (s->pixeis[i] == NULL)  continue;
        
            for (int j = 0; j < s->screen_size.x; j++) 
            {
                if (s->pixeis[i][j] == NULL) continue;

                Pixel_Stack* ps = s->pixeis[i][j];
                Pixel_Node* atual = ps->topo;
                
                while (atual != NULL) 
                {
                    Pixel_Node* anterior = atual->anterior;
                    free(atual);
                    atual = anterior;
                }
                
                free(ps);
                s->pixeis[i][j] = NULL;
            }
            free(s->pixeis[i]);
            s->pixeis[i] = NULL;
        }
        free(s->pixeis); 
        s->pixeis = NULL;
    }

    free(s); 
}

Animation criar_anim(int* frame_list, int qtd_frames, char nome[])
{
    Animation anim;
    strcpy(anim.nome, nome);
    anim.frame_atual = 0;
    anim.qtd_frames = qtd_frames;
    anim.frame_index = frame_list;
    return anim;
}

int* obj_complexo_contem_Pixel_em(ObjetoComplexo* obj, Vector2 pos, int* out_qtd_de_frames) 
{
    int* frames_que_contem = (int*)malloc(obj->qtd_frames * sizeof(int));
    int qtd_frames = 0;

    for (int i = 0; i < obj->qtd_frames; i++) 
        if (obj_contem_Pixel_em(obj->frames[i], pos)) 
            frames_que_contem[qtd_frames++] = i;

    *out_qtd_de_frames = qtd_frames;

    if (qtd_frames == 0)
    {
        free(frames_que_contem);
        return NULL;
    }

    return (int*)realloc(frames_que_contem, qtd_frames * sizeof(int));
}

bool vetor_aponta_para_area_visivel(Screen* s, Vector2 vet, Vector2* out_pos_rel)
{
    Vector2 rel = vector_sum(vector_subtr(vet, s->position), centro_da_tela(s));
    if (out_pos_rel != NULL)
        *out_pos_rel = rel;
    return vetor_valido_na_tela(s, rel);
}

bool vetor_valido_na_tela(Screen *s, Vector2 vet)
{
    if (vet.x < 0 || vet.x >= s->screen_size.x || 
        vet.y < 0 || vet.y >= s->screen_size.y)
        return false;
    return true;
}

bool compare_vector (Vector2 v1, Vector2 v2)
{
    bool x = v1.x == v2.x;
    bool y = v1.y == v2.y;
    return x && y;
}

int compare_color(Color c1, Color c2)
{
    int dist_r = (c1.r - c2.r) * (c1.r - c2.r);
    int dist_g = (c1.g - c2.g) * (c1.g - c2.g);
    int dist_b = (c1.b - c2.b) * (c1.b - c2.b);
    return dist_r + dist_g + dist_b;
}

bool obj_contem_Pixel_em (Objeto* obj, Vector2 pos)
{
    for (int i = 0; i < obj->qtd_Pixel; i++)
        if (compare_vector(pos, obj->info[i].position))
            return true;
    
    return false;
}

Pixel get_pixel_em(Screen* s, Vector2 pos)
{
    Vector2 rel_pos = vector_sum(pos, centro_da_tela(s));
    if (vetor_valido_na_tela(s, rel_pos))
        return s->pixeis[rel_pos.x][rel_pos.y]->topo->pixel;
    else
    {
        moveCursor(new_Vector2(0, s->screen_size.y + 3));
        printf("Tentou obter pixel na posicao {%d, %d}, que está fora dos limites\n", rel_pos.x, rel_pos.y);
        return (Pixel){};
    }
}

// INTERNA
AnimationManager* criar_anim_manager(int qtd_anims)
{
    AnimationManager* anm = (AnimationManager*)malloc(sizeof(AnimationManager));
    anm->anim_atual = 0;
    anm->qtd_anims = qtd_anims;
    return anm;
}

void setar_animation_via_index(ObjetoComplexo* obj, int anim_index)
{
    if (anim_index < 0 || anim_index >= obj->anim_manager->qtd_anims) return;    
    obj->anim_manager->anim_atual = anim_index;
}

void setar_animation_via_nome(ObjetoComplexo* obj, char nome[])
{
    AnimationManager* manager = obj->anim_manager;
    for (int i = 0; i < manager->qtd_anims; i++)
    {
        if (strcmp(nome, manager->anims[i].nome) == 0)
            return setar_animation_via_index(obj, i);
    }
}

void setar_frame_obj_complexo(Screen* s, ObjetoComplexo* obj, int frame)
{
    if (obj->animar) return;
    if (frame < 0 || frame >= obj->qtd_frames) return;
    bool redesenhar = false;
    if (obj->renderizado)
    {
        redesenhar = true;
        esconder_objeto_complexo(s, obj);
    }
    obj->frame_atual = frame;
    if (redesenhar)
        desenhar_objeto_complexo(s, obj);
}

void animar_objeto_complexo(Screen* s, ObjetoComplexo* obj)
{
    if (!obj->animar) return;
    Animation* anim_atual = &(obj->anim_manager->anims[obj->anim_manager->anim_atual]);
    anim_atual->frame_atual = next_anim(anim_atual);
    bool redesenhar = false;
    if (obj->renderizado)
    {
        esconder_objeto_complexo(s, obj);
        redesenhar = true;
    }
    obj->frame_atual = anim_atual->frame_index[anim_atual->frame_atual];
    if (redesenhar)
        desenhar_objeto_complexo(s, obj);
}

void setup_animations(ObjetoComplexo* obj, Animation anims[], int qtd_anims)
{
    if (anims == NULL || obj == NULL || qtd_anims == 0) return;
    obj->anim_manager = criar_anim_manager(qtd_anims);
    obj->anim_manager->anims = anims;
}

void alterar_pivot_obj(Objeto* obj, Vector2 new_pivot)
{
    for (int i = 0; i < obj->qtd_Pixel; i++)
        obj->info[i].position = vector_subtr(obj->info[i].position, new_pivot);
    
    Vector2 min = obj->info[0].position;
    Vector2 max = obj->info[0].position;

    for (int i = 1; i < obj->qtd_Pixel; i++) 
    {
        if (obj->info[i].position.x < min.x) min.x = obj->info[i].position.x;
        if (obj->info[i].position.y < min.y) min.y = obj->info[i].position.y;
        if (obj->info[i].position.x > max.x) max.x = obj->info[i].position.x;
        if (obj->info[i].position.y > max.y) max.y = obj->info[i].position.y;
    }

    obj->size = new_Vector2(max.x - min.x, max.y - min.y);
}

void alterar_pivot_frame(ObjetoComplexo* obj, int frame, Vector2 novo_pivot)
{
    obj->pivot_frames[frame] = novo_pivot;
    Objeto* frame_obj = obj->frames[frame];

    for (int i = 0; i < frame_obj->qtd_Pixel; i++)
        frame_obj->info[i].position = vector_sum(frame_obj->info[i].position, novo_pivot);

    Vector2 min = frame_obj->info[0].position;
    Vector2 max = frame_obj->info[0].position;

    for (int i = 1; i < frame_obj->qtd_Pixel; i++) 
    {
        if (frame_obj->info[i].position.x < min.x) min.x = frame_obj->info[i].position.x;
        if (frame_obj->info[i].position.y < min.y) min.y = frame_obj->info[i].position.y;
        if (frame_obj->info[i].position.x > max.x) max.x = frame_obj->info[i].position.x;
        if (frame_obj->info[i].position.y > max.y) max.y = frame_obj->info[i].position.y;
    }

    frame_obj->size = new_Vector2(max.x - min.x, max.y - min.y);

    Vector2 maior = VETOR_NULO;
    for (int i = 0; i < obj->qtd_frames; i++) 
    {
        if (obj->frames[i]->size.x > maior.x)
            maior.x = obj->frames[i]->size.x;
        if (obj->frames[i]->size.y > maior.y)
            maior.y = obj->frames[i]->size.y;
    }
    obj->size = maior;
}

void desenhar_objeto(Screen* s, Objeto* obj)
{
    if (obj->renderizado) return;

    Vector2 center = centro_da_tela(s);
    char desenhou = 0;

    for (int i = 0; i < obj->qtd_Pixel; i++)
    {
        Vector2 abs_px = get_abs_Pixel_pos(obj, i);
        Vector2 rel = vector_subtr(abs_px, s->position);
        Vector2 pixel_pos = vector_sum(rel, center);
        if (vetor_valido_na_tela(s, pixel_pos))
        {
            desenhou = 1;
            add_Pixel(s->pixeis[pixel_pos.y][pixel_pos.x], obj->info[i]);
        }
    }

    if (desenhou) obj->renderizado = true;
}

void aplicar_filtro_obj(Objeto *obj, Color filtro)
{
    for (int i = 0; i < obj->qtd_Pixel; i++)
        obj->info[i].cor = aplicar_filtro(obj->info[i].cor, filtro);
}

void espelhar_objeto(Screen *s, Objeto *obj, bool horizontalmente)
{
    bool redesenhar = false;
    if (obj->renderizado)
    {
        esconder_objeto(s, obj);
        redesenhar = true;
    }
    
    Vector2 centro = centro_do_objeto(obj);
    for (int i = 0; i < obj->qtd_Pixel; i++)
    {
        Vector2 pos_relativa_centro = vector_sum(obj->info[i].position, reverse_vector(centro));
        if (horizontalmente)
            pos_relativa_centro.x = -pos_relativa_centro.x;
        else
            pos_relativa_centro.y = -pos_relativa_centro.y;
        obj->info[i].position = vector_sum(centro, pos_relativa_centro);
    }
    normalizar_objeto(obj);
    if (redesenhar)
        desenhar_objeto(s, obj);
}

void espelhar_objeto_complexo (Screen* s, ObjetoComplexo* obj, bool horizontalmente)
{
    bool redesenhar = false;

    if (obj->renderizado)
    {
        esconder_objeto_complexo(s, obj);
        redesenhar = true;
    }
    for (int i = 0; i < obj->qtd_frames; i++)
    {
        espelhar_objeto(s, obj->frames[i], horizontalmente);
        if (!compare_vector(obj->pivot_frames[i], VETOR_NULO))
            alterar_pivot_frame(obj, i, obj->pivot_frames[i]);     
    }
    if (redesenhar)
        desenhar_objeto_complexo(s, obj);
}

void desenhar_objeto_complexo (Screen* s, ObjetoComplexo* obj)
{
    if (obj->renderizado) return;
    int frame = obj->frame_atual;
    
    Vector2 center = centro_da_tela(s);
    char desenhou = '0';
    for (int i = 0; i < obj->frames[frame]->qtd_Pixel; i++)
    {
        Vector2 abs_px = get_complexo_abs_Pixel_pos(obj, frame, i);
        Vector2 rel = vector_subtr(abs_px, s->position);
        Vector2 pixel_pos = vector_sum(rel, center);
        if (vetor_valido_na_tela(s, pixel_pos))
        {
            desenhou = '1';
            add_Pixel(s->pixeis[pixel_pos.y][pixel_pos.x], obj->frames[frame]->info[i]);
        }
    }

    if (desenhou == '1') obj->renderizado = true;
}

// INTERNA
void excluir_anim_manager(AnimationManager* manager)
{
    for (int i = 0; i < manager->qtd_anims; i++)
        free(manager->anims[i].frame_index);
    free(manager->anims);
    free(manager);
}

void excluir_objeto (Objeto* obj)
{
    free(obj->info);
    free(obj);
}

void excluir_objeto_complexo(ObjetoComplexo* obj)
{
    for (int i = 0; i < obj->qtd_frames; i++)
        excluir_objeto(obj->frames[i]);
    excluir_anim_manager(obj->anim_manager);
    free(obj->pivot_frames);
    free(obj->frames);
    free(obj);
}

void esconder_objeto(Screen* s, Objeto* obj)
{
    if (!obj->renderizado) return;
    Vector2 center = new_Vector2(
        s->screen_size.x/2,
        s->screen_size.y/2
    );
    for (int i = 0; i < obj->qtd_Pixel; i++)
    {
        Vector2 abs_px = get_abs_Pixel_pos(obj, i);
        Vector2 rel = vector_subtr(abs_px, s->position);
        Vector2 pixel_pos = vector_sum(rel, center);

        if (vetor_valido_na_tela(s, pixel_pos))
            deletar_Pixel(s, pixel_pos);
    }
    obj->renderizado = false;
}

void esconder_objeto_complexo (Screen* s, ObjetoComplexo* obj)
{
    if (obj->renderizado == false) return;
    Vector2 center = new_Vector2(
        s->screen_size.x/2,
        s->screen_size.y/2
    );
    int frame = obj->frame_atual;
    for (int i = 0; i < obj->frames[obj->frame_atual]->qtd_Pixel; i++)
    {
        Vector2 abs_px = get_complexo_abs_Pixel_pos(obj, frame, i);
        Vector2 rel = vector_subtr(abs_px, s->position);
        Vector2 pixel_pos = vector_sum(rel, center);
        if (vetor_valido_na_tela(s, pixel_pos))
            deletar_Pixel(s, pixel_pos);
    }
    obj->renderizado = false;
}

void mover_objeto (Screen* s, Objeto* obj, Vector2 direction)
{
    esconder_objeto(s, obj);
    obj->position = vector_sum(obj->position, direction);
    desenhar_objeto(s, obj);
}

void teleportar_objeto(Screen* s, Objeto* obj, Vector2 pos_final)
{
    esconder_objeto(s, obj);
    obj->position = pos_final;
    desenhar_objeto(s, obj);
}

void teleportar_objeto_complexo(Screen* s, ObjetoComplexo* obj, Vector2 pos_final)
{
    esconder_objeto_complexo(s, obj);
    obj->position = pos_final;
    desenhar_objeto_complexo(s, obj);
}

void preencher_background (Screen* s, Color cor)
{
    for (int i = 0; i < s->screen_size.y; i++)
        for (int j = 0; j < s->screen_size.x; j++)
            add_Pixel(s->pixeis[i][j], criar_Pixel(cor, new_Vector2(i, j)));
}

void printPixel(Pixel_Stack* p) 
{
    if (p == NULL || p->topo == NULL) return;
    Color c = p->topo->pixel.cor;
    printf("\033[38;2;%d;%d;%dm@\033[0m", c.r, c.g, c.b);
}

void mover_objeto_complexo(Screen* s, ObjetoComplexo* obj, Vector2 direction)
{
    esconder_objeto_complexo(s, obj);
    obj->position = vector_sum(obj->position, direction);
    desenhar_objeto_complexo(s, obj);
}

void rotacionar_objeto(Objeto *obj, Vector2 pivot, float graus)
{
    Vector2 tam = obj->size;
    bool grid[tam.x][tam.y];
    memset(grid, false, sizeof(grid));

    Pixel* novos_pixeis = (Pixel*)malloc(obj->qtd_Pixel * sizeof(Pixel));
    int novo_count;

    for (int i = 0; i < obj->qtd_Pixel; i++)
    {
        Vector2 nova_pos = aplicar_matriz_rot(obj->info[i].position, pivot, graus);
        if (!grid[nova_pos.x][nova_pos.y])
        {
            grid[nova_pos.x][nova_pos.y] = true;
            Pixel novo_px = criar_Pixel(obj->info[i].cor, nova_pos);
            novo_px.obj_source = (void**)obj;
            novo_px.source_type = 'o';
            novos_pixeis[novo_count++] = novo_px;
        }
    }
    free(obj->info);
    obj->info = novos_pixeis;
    obj->qtd_Pixel = novo_count;
}

void rotacionar_objeto_complexo (ObjetoComplexo* obj, Vector2 pivot, float graus)
{
    for (int i = 0; i < obj->qtd_frames; i++)
        rotacionar_objeto(obj->frames[i], pivot, graus);
}

void moveCursor(Vector2 v)
{
    printf("\033[%d;%dH", v.y + 1, v.x + 1);
}

void render(Screen* s, bool reset) 
{
    printf("\033[?25l");
    if (reset)
        moveCursor(VETOR_NULO);

    for (int i = 0; i < s->screen_size.y; i++) 
    {
        bool linha_igual = true;
        for (int j = 0; j < s->screen_size.x; j++) 
        {
            Color nova_cor = s->pixeis[i][j]->topo->pixel.cor;
            if (compare_color(s->buffer[i][j], nova_cor) > s->limiar_de_cor || compare_color(s->buffer[i][j], COR_NULA) == 0)
            {
                linha_igual = false;
                break;
            }
        }

        if (linha_igual)
        {
            moveCursor((Vector2){0, i + 1});
            continue;
        }

        moveCursor((Vector2){0, i});
        for (int j = 0; j < s->screen_size.x; j++)
        {
            s->buffer[i][j] = s->pixeis[i][j]->topo->pixel.cor;
            printPixel(s->pixeis[i][j]);
        }
    }
    for (int i = 0; i < s->screen_size.x; i++)
        printf("-");
    printf("\n");
    printf("\033[?25h");
}

void aplicar_filtro_obj_complexo (ObjetoComplexo* obj, Color filtro)
{
    for (int i = 0; i < obj->qtd_frames; i++)
        aplicar_filtro_obj(obj->frames[i], filtro);
}

//INTERNA
Pixel* converter_piskel_frame_para_Pixel_info(const uint32_t frame_data[], int width, int height, int* qtd_Pixel) 
{
    int total_Pixels = width * height;
    int pixeis_coloridos = 0;

    for (int i = 0; i < total_Pixels; i++)
        if(frame_data[i] != 0x00000000) 
            pixeis_coloridos++;

    Pixel* info = malloc(pixeis_coloridos * sizeof(Pixel));
    if (info == NULL) 
    {
        perror("Falha ao alocar memória para piskel frame");
        exit(EXIT_FAILURE);
    }

    int idx = 0;
    for (int i = 0; i < total_Pixels; i++) 
    {
        if(frame_data[i] == 0x00000000)
            continue;
        int x = i % width;
        int y = i / width;
        Color cor = converter_ABGR_para_Color(frame_data[i]);
        info[idx++] = criar_Pixel(cor, new_Vector2(x, y));
    }
    
    *qtd_Pixel = pixeis_coloridos;
    return info;
}

Objeto* criar_piskel_obj(const uint32_t frame_data[], int width, int height)
{
    int size;
    Pixel* info = converter_piskel_frame_para_Pixel_info(frame_data, width, height, &size);
    return criar_objeto_custom(info, size, true);
}

ObjetoComplexo* criar_objeto_complexo_piskel(int qtd_frames, int width, int height, const uint32_t obj_data[qtd_frames][width * height])
{
    Objeto** obj_set = (Objeto**)malloc(qtd_frames * sizeof(Objeto*));
    for (int i = 0; i < qtd_frames; i++)
        obj_set[i] = criar_piskel_obj(obj_data[i], width, height);
    return criar_objeto_complexo_via_lista(obj_set, qtd_frames);
}

ObjetoComplexo* criar_objeto_complexo_via_lista(Objeto** obj_origem, int qtd_objetos)
{
    ObjetoComplexo* obj = (ObjetoComplexo*)malloc(sizeof(ObjetoComplexo));
    obj->pivot_frames = (Vector2*)malloc(qtd_objetos * sizeof(Vector2));
    obj->position = VETOR_NULO;
    obj->frame_atual = 0;
    obj->renderizado = false;
    obj->qtd_frames = qtd_objetos;
    obj->animar = true;
    obj->anim_manager = NULL;
    for (int i = 0; i < qtd_objetos; i++)
    {
        obj_origem[i]->position = VETOR_NULO;
        obj_origem[i]->renderizado = false;
        normalizar_objeto(obj_origem[i]);
    }
    obj->frames = obj_origem;
    Vector2 maior = VETOR_NULO;
    for (int i = 0; i < qtd_objetos; i++)
    {
        obj->pivot_frames[i] = VETOR_NULO;
        if (obj->frames[i]->size.x > maior.x)
            maior.x = obj->frames[i]->size.x;
        if (obj->frames[i]->size.y > maior.y)
            maior.y = obj->frames[i]->size.y;
    }
    obj->size = maior;
    for (int i = 0; i < qtd_objetos; i++) for (int j = 0; j < obj->frames[i]->qtd_Pixel; j++)
    {
        obj->frames[i]->info[j].obj_source = (void**)obj;
        obj->frames[i]->info[j].source_type = 'c';
    }
    
    return obj;
}
