/**
 * @file 02_piskel.c
 * @brief Demonstra integração com sprites exportados do Piskel
 */

#include <graphycs.h>
#include "assets/Smile.h"  // Arquivo exportado do Piskel (renomeado para .h)

int main() 
{
    // 1. Inicialização
    Screen* tela = criar_tela(new_Vector2(90, 30), COLOR_CYAN);
    Objeto* quadrado = criar_retangulo_monocromatico(COLOR_RED, new_Vector2(10, 10));
    centralizar_objeto(quadrado);
    desenhar_objeto(tela, quadrado);

    // 2. Objeto Piskel (sorriso)
    Objeto* sorriso = criar_piskel_obj(
        smile_data[0],          // Dados do frame (array exportado pelo Piskel)
        SMILE_FRAME_WIDTH,      // Largura do frame (definido no Smile.h)
        SMILE_FRAME_HEIGHT      // Altura do frame (definido no Smile.h)
    );
    
    if (!sorriso) {
        fprintf(stderr, "Erro ao carregar sprite do Piskel!\n");
        excluir_objeto(quadrado);
        return 1;
    }

    // 3. Posicionamento e renderização
    desenhar_objeto(tela, quadrado);
    teleportar_objeto(tela, sorriso, new_Vector2(10, -2));  // Desenha automaticamente
    render(tela, true);  // 'true' para sobrescrever o frame anterior

    // 4. Finalização
    printf("Pressione qualquer tecla para sair...\n");
    getchar();
    
    excluir_objeto(quadrado);
    excluir_objeto(sorriso);

    return 0;
}