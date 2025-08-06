/**
 * @file 03_animacao.c
 * @brief Demonstra configuração e animação de objetos complexos com múltiplas animações
 * @details Mostra:
 * - Carregamento de spritesheet do Piskel
 * - Criação de animações customizadas
 * - Controle de animações por nome e índice
 * - Modificação de pivôs de frames
 */

#include <graphycs.h>
#include <windows.h>
#include "assets/personagem.h"

int main() 
{
    // 1. Inicialização
    Screen* tela = criar_tela(new_Vector2(100, 30), COLOR_BRANCO, 25);

    // 2. Criação do objeto complexo com verificação
    ObjetoComplexo* personagem = criar_objeto_complexo_piskel(
        PERSONAGEM_FRAME_COUNT, 
        PERSONAGEM_FRAME_WIDTH,
        PERSONAGEM_FRAME_HEIGHT,
        personagem_piskel_data
    );
    
    if (!personagem) {
        fprintf(stderr, "Erro ao carregar personagem!\n");
        return 1;
    }

    // 3. Configuração das animações
    int frames_parado[] = {0, 1};
    int frames_andando[] = {2, 3};
    int frames_extra[] = {4, 5};

    Animation animacoes[] = {
        criar_anim(frames_parado, 2, "parado"),
        criar_anim(frames_andando, 2, "andando"),
        criar_anim(frames_extra, 2, "extra")
    };

    setup_animations(personagem, animacoes, 3);

    // Ajuste de pivô para o frame 1
    alterar_pivot_frame(personagem, 1, new_Vector2(0, 1));

    // 4. Loop principal de animação
    desenhar_objeto_complexo(tela, personagem);
    int contador = 0;
    while (1) {
        // Renderização
        render(tela, true);
        
        // Lógica de troca de animação
        if (contador > 60) {
            setar_animation_via_nome(personagem, "parado");
            contador = 0;
        }
        else if (contador > 40) {
            setar_animation_via_nome(personagem, "extra");
        } 
        else if (contador > 20) {
            setar_animation_via_nome(personagem, "andando");
        }

        // Animação e input
        animar_objeto_complexo(tela, personagem);
        contador++;
        
        // Controle de FPS e saída
        Sleep(100);
        
        if (ler_teclado() != '\0') {
            break;
        }
    }

    // 5. Limpeza
    excluir_objeto_complexo(personagem);
    return 0;
}