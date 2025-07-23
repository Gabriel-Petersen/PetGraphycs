/**
 * @file 01_basico.c
 * @brief Demonstra como inicializar e a função básica de criação e desenho de objetos
 */
#include <graphycs.h>

int main() 
{
    // 1. Cria a tela principal (120x30 pixels, fundo ciano)
    Screen* tela = criar_tela(new_Vector2(120, 30), COLOR_CYAN);
    if (!tela) 
    {
        fprintf(stderr, "Erro ao criar tela!\n");
        return 1;
    }

    // 2. Cria um quadrado vermelho (10x10 pixels)
    Objeto* quadrado = criar_retangulo_monocromatico(COLOR_RED, new_Vector2(10, 10));
    if (!quadrado) 
    {
        fprintf(stderr, "Erro ao criar objeto!\n");
        excluir_tela(tela);
        return 1;
    }

    // 3. Centraliza o pivô do quadrad na tela (padrão = canto superior esquerdo)
    centralizar_objeto(quadrado);

    // 4. Renderização
    desenhar_objeto(tela, quadrado);
    // 'true' reposiciona o cursor em (0, 0), sobrescrevendo a tela anterior.
    // Use 'false' apenas para renderizações avançadas (ex: multi-telas).
    render(tela, true);

    // 5. Espera entrada do usuário antes de fechar
    printf("Pressione qualquer tecla para sair...\n");
    getchar();

    // 6. Libera memória
    excluir_objeto(quadrado);
    excluir_tela(tela);

    return 0;
}