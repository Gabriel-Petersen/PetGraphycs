#ifndef GRAPHYCS_H
#define GRAPHYCS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <stdint.h>
#include <ctype.h>
#include <limits.h>

/** @defgroup BasicTypes Tipos Básicos
 *  @brief Definições de estruturas fundamentais
 *  @{
 */

/**
 * @brief Estrutura de cores no padrão RGB.
 * @note Nem todo terminal suporta o padrão RGB. Requer teste
 */
typedef struct {
    int r; /**< Componente vermelho (0-255). */
    int g; /**< Componente verde (0-255). */
    int b; /**< Componente azul (0-255). */
} Color;

/**
 * @brief Estrutura para coordenadas 2D (x, y).
 */
typedef struct {
    int x; /**< Coordenada horizontal (sentido convencional). */
    int y; /**< Coordenada vertical (sentido invertido). */
} Vector2;
/** @} */

/** @defgroup ColorConstants Constantes de Cores
 *  @brief Macros para cores pré-definidas
 *  @{
 */

/** @brief Cor preta (RGB: 0, 0, 0). */
#define COLOR_BLACK (Color){0, 0, 0}
/** @brief Cor vermelha (RGB: 255, 0, 0). */
#define COLOR_RED   (Color){255, 0, 0}
/** @brief Cor verde (RGB: 0, 255, 0). */
#define COLOR_GREEN   (Color){0, 255, 0}
/** @brief Cor amarela (RGB: 255, 255, 0). */
#define COLOR_YELLOW  (Color){255, 255, 0}
/** @brief Cor azul (RGB: 0, 0, 255). */
#define COLOR_BLUE    (Color){0, 0, 255}
/** @brief Cor roxa (RGB: 128, 0, 128). */
#define COLOR_PURPLE  (Color){128, 0, 128}
/** @brief Cor ciano (RGB: 0, 255, 255). */
#define COLOR_CYAN    (Color){0, 255, 255}
/** @brief Cor branca (RGB: 255, 255, 255). */
#define COLOR_WHITE   (Color){255, 255, 255}
/** @} */

/** @defgroup VectorConstants Constantes de Vetores
 *  @brief Macros para vetores unitários e nulo
 *  @{
 */
/**
 * @brief Vetor nulo (0, 0).
 */
#define VETOR_NULO (Vector2){0, 0}
/**
 * @brief Vetor unitário para cima (0, -1).
 * @note O eixo Y cresce para baixo em muitos sistemas gráficos.
 */
#define VETOR_CIMA (Vector2){0, -1}
/**
 * @brief Vetor unitário para baixo (0, 1).
 * @note O eixo Y cresce para baixo em muitos sistemas gráficos.
 */
#define VETOR_BAIXO (Vector2){0, 1}
/**
 * @brief Vetor unitário para esquerda (-1, 0).
 */
#define VETOR_ESQUERDA (Vector2){-1, 0}
/**
 * @brief Vetor unitário para direita (1, 0).
 */
#define VETOR_DIREITA (Vector2){1, 0}
/** @} */


/** @defgroup PixelModel Modelo de Pixel
 *  @brief Definição de Pixel, nó e pilha de pixels
 *  @{
 */
/**
 * @brief Estrutura que representa um Pixel na tela.
 * @details Combina uma cor (Color) e uma posição (Vector2).
 */
typedef struct {
    Color cor;      /**< Cor do pixel. */
    Vector2 position; /**< Posição do pixel. */
    void** obj_source; /**< Objeto ou ObjetoComplexo de onde veio este pixel*/
    char source_type; /**< 'o' para Objeto; 'c' para ObjetoComplexo */
} Pixel;
/**
 * @brief Nodo da pilha dinâmica de pixeis
 */
typedef struct Pixel_Node{
    Pixel pixel;
    struct Pixel_Node* anterior;
} Pixel_Node;
/**
 * @brief Estrutura que representa a pilha de pixeis de cada posição da tela. Cada coordenada empilha pixeis e renderiza o do topo.
 */
typedef struct {
    Pixel_Node* topo;
} Pixel_Stack;
/** @} */
/**
 * @brief Estrutura da tela sob a qual os objetos serão renderizados
 * @details Defina o tamanho da tela de acordo com o tamanho do monitor/terminal a ser utilizado
 */
typedef struct {
    Pixel_Stack*** pixeis;  /**< Matriz de ponteiros para pilhas dinâmicas de pixeis */
    Vector2 screen_size;
    Vector2 position;
} Screen;

/** @defgroup AnimationModel Modelo de Animação
 *  @brief Estruturas para frames e gerenciador de animações
 *  @{
 */

/**
 * @brief Representa uma animação específica de um ObjetoComplexo.
 * @details Armazena os índices dos frames que compõem a animação e controla o frame atual.
 * @note Exemplo: Se um personagem tem uma animação "Andar" com frames 4-7, 
 *       `frame_index` armazenará [4,5,6,7], e `qtd_frames` será 4.
 */
typedef struct {
    char nome[20];      /**< Nome da animação (ex: "Parado", "Andar"). */
    int* frame_index;   /**< Array com os índices dos frames desta animação. */
    int qtd_frames;     /**< Quantidade de frames na animação. */
    int frame_atual;    /**< Frame atual sendo exibido (índice dentro de `frame_index`). */
} Animation;

/**
 * @brief Gerencia múltiplas animações de um ObjetoComplexo.
 * @details Controla qual animação está ativa e permite alternar entre elas.
 * @see Animation
 */
typedef struct {
    int qtd_anims;      /**< Quantidade total de animações disponíveis. */
    int anim_atual;     /**< Índice da animação atualmente em execução. */
    Animation* anims;   /**< Array de animações (cada uma com seus próprios frames). */
} AnimationManager;
/** @} */

/** @defgroup ObjectModel Modelo de Objetos
 *  @brief Estruturas de objetos simples e complexos
 *  @{
 */
/**
 * @brief Representa um objeto gráfico estático (um único frame).
 * @details Um objeto é um conjunto de pixels cujas posições são relativas à posição do objeto.
 */
typedef struct {
    Vector2 position;   /**< Posição global do objeto no espaço 2D. */
    Pixel* info;        /**< Vetor de pixeis que formam a imagem do objeto. */
    Vector2 size;       /**< Dimensões do objeto (largura, altura). */
    int qtd_Pixel;      /**< Quantidade de pixeis no objeto. */
    bool renderizado;   /**< Flag que indica se o objeto está renderizado. */
} Objeto;

/**
 * @brief Representa um objeto com múltiplos frames e animações (ex: personagens de jogos).
 * @details Um ObjetoComplexo é um "vetor de Objetos", onde cada Objeto é um frame. O AnimationManager controla quais frames compõem cada animação.
 */
typedef struct {
    Vector2 position;           /**< Posição global do objeto. */
    Vector2* pivot_frames;      /**< Pontos de pivô customizados para cada frame (opcional, default = VETOR_NULO). */
    Objeto** frames;           /**< Array de ponteiros para Objetos (frames individuais). */
    AnimationManager* anim_manager; /**< Gerenciador de animações. */
    Vector2 size;              /**< Dimensões do objeto. */
    bool renderizado;          /**< Flag de renderização. */
    bool animar;               /**< Se verdadeiro, avança automaticamente os frames. */
    int qtd_frames;            /**< Quantidade total de frames. */
    int frame_atual;           /**< Frame atual sendo exibido. */
} ObjetoComplexo;
/** @} */

/** @defgroup InputOutput Entrada e Saída
 *  @brief Funções de teclado e criação de tela
 *  @{
 */
/**
 * @brief Lê o input do teclado sem interromper a execução do programa
 * @return A tecla atual pressionada, em uppercase, ou '\0' se nada pressionado
 */
char ler_teclado();

/**
 * @brief Cria o objeto de tela que será renderizado
 * @param tamanho O tamanho da tela em quantidade de pixeis (depende do tamanho do terminal utilizado)
 * @param fundo Preenche a base da matriz de pilhas de pixeis com um retângulo monocromático com a cor 'fundo'
 * @return O objeto da tela, devidamente configurado
 */
Screen* criar_tela(Vector2 tamanho, Color fundo);
/** @} */

/** @defgroup ScreenOperations Operações de Tela
 *  @brief Movimentação, limpeza e renderização
 *  @{
 */
void mover_tela (Screen* s, Vector2 direction);
/**
 * @brief Preenche toda a tela com uma cor de fundo.
 * @param s Ponteiro para a tela.
 * @param cor Cor de preenchimento.
 */
void preencher_background(Screen* s, Color cor);
/**
 * @brief Renderiza o conteúdo da tela no console.
 * @param s Ponteiro para a tela.
 * @param reset Se true, limpa a tela antes de desenhar.
 */
void render(Screen* s, bool reset);
/**
 * @brief Move o cursor de renderização para a posição dada.
 * @param v Nova posição do cursor.
 */
void moveCursor(Vector2 v);
/** @} */

/**
 * @defgroup ColorOperations Operações com Cores RGB
 * @brief Funções para manipulação de cores RGB.
 * @{
 */

/**
 * @brief Retorna uma nova cor dados os componentes RGB
 * @return Estrutura de cor os valores dados
 */
Color criar_cor(int r, int g, int b);

/**
 * @brief Põe uma cor de filtro em cima de outra cor para alterar sua matiz
 * @param original A cor original
 * @param filtro A cor que servirá de filtro
 * @note Não altera a cor original, cria uma nova cor filtrada
 * @return Cor filtrada
 */
Color aplicar_filtro(Color original, Color filtro);
/** @} */


/** @defgroup VectorOperations Operações com Vetores
 *  @brief Funções auxiliares para Vector2
 *  @{
 */

/**
 * @brief Retorna um novo vetor dados x e y
 * @return Novo vetor resultante (x, y).
 */
Vector2 new_Vector2(int x, int y);
/**
 * @brief Calcula o produto de um vetor por um escalar inteiro.
 * @param vet Vetor de entrada (Vector2).
 * @param escalar Valor inteiro para multiplicação.
 * @return Novo vetor resultante (vet.x * escalar, vet.y * escalar).
 */
Vector2 produto_vetor_escalar(Vector2 vet, float escalar);

/**
 * @brief Soma dois vetores.
 * @param v1 Primeiro vetor.
 * @param v2 Segundo vetor.
 * @return Vetor resultante (v1.x + v2.x, v1.y + v2.y).
 */
Vector2 vector_sum(Vector2 v1, Vector2 v2);

/**
 * @brief Subtrai dois vetores.
 * @param v1 Primeiro vetor.
 * @param v2 Segundo vetor.
 * @return Vetor resultante (v1.x - v2.x, v1.y - v2.y).
 */
Vector2 vector_subtr(Vector2 v1, Vector2 v2);

/**
 * @brief Inverte os componentes de um vetor.
 * @details Transforma (x, y) em (-x, -y).
 * @param v Vetor de entrada.
 * @return Vetor invertido.
 */
Vector2 reverse_vector (Vector2 v);
/** @} */

/** @defgroup PixelPosition Posição de Pixels
 *  @brief Cálculo de posições absolutas em objetos
 *  @{
 */
/**
 * @brief Retorna a posição absoluta do index-ésimo píxel do vetor de pixeis dentro do Objeto
 * @details Seja P o vetor de pixeis, retorna a soma vetorial de Obj.position + P[index].position
 * @param obj Objeto dono do pixel
 * @param index Índice do píxel dentro do vetor de pixeis
 * @return Vetor para a posição absoluta daquele pixel na tela.
 */
Vector2 get_abs_Pixel_pos(Objeto* obj, int index);

/**
 * @brief Retorna a posição absoluta do index-ésimo píxel do vetor de pixeis do frame-ésimo frame dentro do Objeto Complexo
 * @param obj Objeto Complexo dono do pixel
 * @param frame Frame do objeto complexo que contém o pixel
 * @param index Índice do píxel dentro do vetor de pixeis do frame dado
 * @see get_abs_Pixel_pos
 * @return Vetor para a posição absoluta daquele pixel na tela.
 */
Vector2 get_complexo_abs_Pixel_pos(ObjetoComplexo* obj, int frame, int index);
/** @} */

/** @defgroup CenterOperations Centro Geométrico
 *  @brief Cálculo do ponto médio de objetos
 *  @{
 */
/**
 * @brief Retorna as coordenadas relativas do centro geométrico do objeto
 * @details Retorna o vetor que parte da posição absoluta do objeto (obj.position) e vai ao centro geométrico do mesmo
 * @return Vetor para a posição relativa do centro geométrico do objeto dado.
 */
Vector2 centro_do_objeto(Objeto* obj);

/**
 * @brief Retorna as coordenadas relativas do centro geométrico do objeto complexo, considerando a totalidade dos frames
 * @details Retorna o vetor que parte da posição absoluta do objeto complexo (obj.position) e vai ao centro geométrico do mesmo
 * @see centro_do_objeto
 * @return Vetor para a posição relativa do centro geométrico do objeto dado, considerando todos os frames.
 */
Vector2 centro_objeto_complexo(ObjetoComplexo* obj);

/**
 * @brief Retorna as coordenadas relativas do centro geométrico da tela
 * @details Retorna o vetor que parte da posição absoluta da tela (s.position) e vai ao centro geométrico da mesma
 * @return Vetor para a posição relativa do centro geométrico da tela.
 */
Vector2 centro_da_tela(Screen* s);
/** @} */

/** @defgroup PixelOperations Operações Diretas com Pixels
 *  @brief Criação, remoção e gerenciamento de pilha
 *  @{
 */

/**
 * @brief Cria um Pixel com cor e posição especificadas.
 * @param cor Cor do pixel.
 * @param pos Posição absoluta ou relativa do pixel.
 * @return Pixel configurado.
 */
Pixel criar_Pixel(Color cor, Vector2 pos);

/**
 * @brief Desempilha o pixel do topo da pilha de pixeis.
 * @param stack Ponteiro para a pilha de pixeis.
 * @return Pixel removido do topo.
 * @internal Usada internamente para gerenciar pilhas de pixels.
 */
Pixel desempilhar_Pixel(Pixel_Stack* stack);

/**
 * @brief Remove o pixel do topo da pilha na tela em posição dada.
 * @param s Ponteiro para a tela.
 * @param pos Posição absoluta da pilha de pixels.
 * @return Pixel removido.
 */
Pixel deletar_Pixel(Screen* s, Vector2 pos);
/** @} */


/** @defgroup SimpleObjectOperations Objetos Simples
 *  @brief Criação e edição de objetos baseados em pixels
 *  @{
 */

/**
 * @brief Cria um objeto customizado a partir de uma lista de pixels.
 * @param info Vetor de pixels (posições relativas a 0,0).
 * @param qtd_Pixel Quantidade de pixels em info.
 * @param normalizar Se true, ajusta pixels para pivot em (0,0).
 * @return Ponteiro para o objeto criado, com posição inicial em (0,0).
 */
Objeto* criar_objeto_custom(Pixel* info, int qtd_Pixel, bool normalizar);

/**
 * @brief Recorta parte de um objeto dentro de um retângulo definido.
 * @param obj Ponteiro para o objeto original.
 * @param inicio Posição relativa do canto de início do retângulo.
 * @param fim Posição relativa do canto de fim do retângulo.
 * @return Ponteiro para o novo objeto recortado.
 */
Objeto* recortar_objeto (Objeto* obj, Vector2 inicio, Vector2 fim);

/**
 * @brief Gera um objeto retangular monocromático preenchido.
 * @param cor Cor do retângulo.
 * @param tamanho Vetor indicando altura (y) e largura (x).
 * @return Ponteiro para o objeto retângulo criado.
 */
Objeto* criar_retangulo_monocromatico(Color cor, Vector2 tamanho);

/**
 * @brief Cria um objeto de eixos cartesianos brancos para debug.
 * @details Desenha cruz centralizada na tela para auxiliar posicionamento.
 * @param s Ponteiro para a tela de referência.
 * @return Ponteiro para o objeto de debug (eixos).
 */
Objeto* criar_obj_eixos_debug(Screen* s);

/**
 * @brief Une dois objetos em um só a partir de um pivot.
 * @note Objetos originais serão liberados da memória.
 * @param prioridade Objeto que mantém prioridade de pixels.
 * @param novo Objeto cujos pixels serão incorporados.
 * @param pivot Vetor relativo do segundo objeto em relação ao primeiro.
 * @return Ponteiro para o novo objeto resultante da fusão.
 */
Objeto* merge_objeto(Objeto* prioridade, Objeto* novo, Vector2 pivot);

/**
 * @brief Cria um objeto a partir de dados de um frame do Piskel.
 * @param frame_data Array de pixels (frame) fornecido pelo Piskel.
 * @param width Largura do frame.
 * @param height Altura do frame.
 * @return Ponteiro para o objeto criado.
 */
Objeto* criar_piskel_obj (const uint32_t frame_data[], int width, int height);
/** @} */

/** @defgroup ComplexObjectCreation Objetos Complexos
 *  @brief Inicialização e configuração básica
 *  @{
 */

/**
 * @brief Cria um objeto complexo a partir de matriz do Piskel.
 * @details Dados completos de frames, sem animações configuradas.
 * @param qtd_frames Número de frames.
 * @param width Largura total dos frames.
 * @param height Altura total dos frames.
 * @param obj_data Matriz [qtd_frames][width*height] com pixels.
 * @see setup_animations
 * @return Ponteiro para o objeto complexo parcialmente configurado.
 */
ObjetoComplexo* criar_objeto_complexo_piskel(int qtd_frames, int width, int height, const uint32_t obj_data[qtd_frames][width * height]);

/**
 * @brief Cria um objeto complexo a partir de lista de objetos (frames).
 * @param obj_origem Vetor de ponteiros para cada frame.
 * @param qtd_objetos Quantidade de frames / tamanho do vetor.
 * @return Ponteiro para o objeto complexo sem animações.
 * @see setup_animations
 */
ObjetoComplexo* criar_objeto_complexo_via_lista(Objeto** obj_origem, int qtd_objetos);

/**
 * @brief Cria uma animação encapsulando índices de frames.
 * @param frame_list Vetor de índices de frames para a animação.
 * @param qtd_frames Quantidade de frames na lista.
 * @param nome Nome da animação (string terminada em '\0').
 * @return Estrutura Animation pronta para uso.
 * @see setup_animations
 */
Animation criar_anim(int* frame_list, int qtd_frames, char nome[]);

/**
 * @brief Configura o gerenciador de animações (AnimationManager) de um objeto complexo.
 * @param obj Ponteiro para o objeto complexo alvo.
 * @param anims Vetor de animações já criadas.
 * @param qtd_anims Quantidade de animações no vetor.
 * @see criar_anim
 * @see AnimationManager
 */
void setup_animations(ObjetoComplexo* obj, Animation anims[], int qtd_anims);
/** @} */

/** @defgroup QueryFunctions Consultas e Predicados
 *  @brief Verificações de limites e presença de pixels
 *  @{
 */
/** @brief retorna o pixel que está no topo da pilha na posição relativa à tela dada
 * @param s A tela a qual será lido o pixel
 * @param pos A posição, relativa ao centro da tela, do pixel a ser lido
 * @return O pixel no topo da pilha na posição dada
 */
Pixel get_pixel_em(Screen* s, Vector2 pos);
/** @brief Retorna true se vetor está dentro dos limites da tela. */
bool vetor_valido_na_tela(Screen* s, Vector2 vet);
/** @brief Compara igualdade entre dois vetores. */
bool compare_vector(Vector2 v1, Vector2 v2);
/** @brief Verifica se objeto simples contém pixel em posição relativa dada. */
bool obj_contem_Pixel_em(Objeto* obj, Vector2 pos);
/**
 * @brief Verifica em quais frames de um objeto complexo há pixel em posição.
 * @param obj Ponteiro para o objeto complexo.
 * @param pos Posição relativa a verificar.
 * @param out_qtd_de_frames Ponteiro para receber a quantidade de frames encontrados.
 * @return Vetor de índices de frames que contêm pixel na posição, ou NULL.
 */
int* obj_complexo_contem_Pixel_em(ObjetoComplexo* obj, Vector2 pos, int* out_qtd_de_frames);
/** @} */

/** @defgroup AnimationControl Controle de Animações
 *  @brief Seleção e execução de animações
 *  @{
 */
/**
 * @brief Seleciona a animação atual de um objeto complexo por índice.
 * @param obj Ponteiro para o objeto complexo.
 * @param anim_index Índice da animação a ativar.
 */
void setar_animation_via_index(ObjetoComplexo* obj, int anim_index);
/**
 * @brief Seleciona a animação atual de um objeto complexo por nome.
 * @param obj Ponteiro para o objeto complexo.
 * @param nome Nome da animação a ativar.
 */
void setar_animation_via_nome(ObjetoComplexo* obj, char nome[]);
/**
 * @brief Executa o passo de animação do objeto complexo na tela.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto complexo.
 */
void animar_objeto_complexo(Screen* s, ObjetoComplexo* obj);
/**
 * @brief Define manualmente o frame atual de um objeto complexo.
 * @note Se o objeto tem animações configuradas, nada acontece
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto complexo.
 * @param frame Índice do frame a setar.
 */
void setar_frame_obj_complexo(Screen* s, ObjetoComplexo* obj, int frame);
/** @} */

/** @defgroup FilterMirrorRotation Filtro, Espelhamento e Rotação
 *  @brief Funções para modificar cor, ângulo e simetria de objetos
 *  @{
 */
/**
 * @brief Aplica um filtro de cor ao objeto simples.
 * @param obj Ponteiro para o objeto.
 * @param filtro Cor do filtro a aplicar.
 */
void aplicar_filtro_obj (Objeto* obj, Color filtro);
/**
 * @brief Aplica um filtro de cor ao objeto complexo.
 * @param obj Ponteiro para o objeto complexo.
 * @param filtro Cor do filtro a aplicar.
 */
void aplicar_filtro_obj_complexo (ObjetoComplexo* obj, Color filtro);
/**
 * @brief Espelha um objeto simples horizontalmente ou verticalmente.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto.
 * @param horizontalmente True para espelho horizontal, false para vertical.
 */
void espelhar_objeto (Screen* s, Objeto* obj, bool horizontalmente);
/**
 * @brief Espelha todos os frames de um objeto complexo horizontalmente ou verticalmente.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto complexo.
 * @param horizontalmente True para espelho horizontal, false para vertical.
 */
void espelhar_objeto_complexo (Screen* s, ObjetoComplexo* obj, bool horizontalmente);
/**
 * @brief Rotaciona um objeto simples em torno de um pivô.
 * @note Eventual perda de pixeis na imagem, função irreversível
 * @param obj Ponteiro para o objeto.
 * @param pivot Ponto de rotação relativo ao objeto.
 * @param graus Ângulo em graus para rotacionar.
 */
void rotacionar_objeto (Objeto* obj, Vector2 pivot, float graus);
/**
 * @brief Rotaciona um objeto complexo em torno de um pivô.
 * @note Eventual perda de pixeis na imagem, função irreversível
 * @param obj Ponteiro para o objeto complexo.
 * @param pivot Ponto de rotação relativo ao objeto.
 * @param graus Ângulo em graus para rotacionar.
 */
void rotacionar_objeto_complexo (ObjetoComplexo* obj, Vector2 pivot, float graus);
/** @} */

/** @defgroup PivotOperations Operações de Pivô
 *  @brief Normalização e ajuste de pivô
 *  @{
 */

/**
 * @brief Normaliza objeto simples para ter pivô no canto superior esquerdo.
 * @param obj Ponteiro para o objeto.
 */
void normalizar_objeto(Objeto* obj);
/**
 * @brief Centraliza o pivô do objeto simples no centro geométrico.
 * @param obj Ponteiro para o objeto.
 */
void centralizar_objeto(Objeto* obj);
/**
 * @brief Altera o pivô de um objeto simples
 * @param obj Ponteiro para o objeto simples.
 * @param new_pivot Nova posição do pivô.
 */
void alterar_pivot_obj(Objeto* obj, Vector2 new_pivot);
/**
 * @brief Altera o pivô de um frame específico em objeto complexo.
 * @param obj Ponteiro para o objeto complexo.
 * @param frame Índice do frame a ajustar.
 * @param novo_pivot Nova posição do pivô relativo ao frame.
 */
void alterar_pivot_frame(ObjetoComplexo* obj, int frame, Vector2 novo_pivot);
/** @} */

/** @defgroup RenderingRender Desenho e Ocultação
 *  @brief Funções para desenhar e esconder objetos na tela
 *  @{
*/
/**
 * @brief Desenha um objeto simples na tela em sua posição.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto.
 */
void desenhar_objeto(Screen* s, Objeto* obj);
/**
 * @brief Desenha um objeto complexo na tela em sua posição.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto complexo.
 */
void desenhar_objeto_complexo (Screen* s, ObjetoComplexo* obj);
/**
 * @brief Esconde um objeto simples removendo seus pixels na tela.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto.
 */
void esconder_objeto(Screen* s, Objeto* obj);
/**
 * @brief Esconde um objeto complexo removendo seus pixels na tela.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto complexo.
 */
void esconder_objeto_complexo (Screen* s, ObjetoComplexo* obj);
/** @} */

/** @defgroup Lifecycle Ciclo de Vida de Objetos
 *  @brief Alocação e liberação de memória
 *  @{
 */
/**
 * @brief Exclui completamente um objeto simples da memória.
 * @param obj Ponteiro para o objeto.
 */
void excluir_objeto(Objeto* obj);
/**
 * @brief Exclui completamente um objeto complexo da memória.
 * @param obj Ponteiro para o objeto complexo.
 */
void excluir_objeto_complexo(ObjetoComplexo* obj);
/**
 * @brief Exclui completamente uma tela da memória.
 * @param s Ponteiro para a tela.
 */
void excluir_tela(Screen* s);
/** @} */

/** @defgroup Transformations Movimentação e Teletransporte
 *  @brief Funções para mover e tele transportar objetos
 *  @{
 */

/**
 * @brief Teleporta um objeto simples para posição final absoluta.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto.
 * @param pos_final Posição final absoluta.
 */
void teleportar_objeto(Screen* s, Objeto* obj, Vector2 pos_final);
/**
 * @brief Teleporta um objeto complexo para posição final absoluta.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto complexo.
 * @param pos_final Posição final absoluta.
 */
void teleportar_objeto_complexo(Screen* s, ObjetoComplexo* obj, Vector2 pos_final);
/**
 * @brief Move um objeto simples de acordo com vetor de deslocamento.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto.
 * @param direction Deslocamento relativo a aplicar.
 */
void mover_objeto(Screen* s, Objeto* obj, Vector2 direction);
/**
 * @brief Move um objeto complexo de acordo com vetor de deslocamento.
 * @param s Ponteiro para a tela.
 * @param obj Ponteiro para o objeto complexo.
 * @param direction Deslocamento relativo a aplicar.
 */
void mover_objeto_complexo(Screen* s, ObjetoComplexo* obj, Vector2 direction);
/** @} */

#endif // GRAPHYCS_H
