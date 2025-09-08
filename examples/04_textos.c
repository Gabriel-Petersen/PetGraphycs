/**
 * @file 04_textos.c
 * @brief Demonstra configuração de textos estáticos in-tela
 * @note O sistema de texto só oferece suporte aos 26 caracteres básicos, os 10 algarismos e os caracteres especiais:
 * Ponto .
 * Vírgula ,
 * Exclamação ?
 * Interrogação !
 */
#include <graphycs.h>

int main ()
{
    // 1. Inicialização
    Screen* tela = criar_tela(new_Vector2(120, 30), COLOR_BRANCO, 0);

    // 2. Criação do objeto de texto
    Objeto* ola_mundo = criar_objeto_de_texto(
        1, // O espaçamento, em pixeis, entre os caracteres do texto
        3, // O tamanho da fonte (só admite 1, 2 ou 3)
        "Hello, World!" // A mensagem de texto que o objeto carrega
    );
    centralizar_objeto(ola_mundo);

    // 3. Edição da cor do texto
    trocar_cor_texto(ola_mundo, COLOR_VERMELHO); // Passe aqui a cor que o texto deve ter - preto por padrão
    // A alteração de cor deve ser feita antes de desenhar
    mover_objeto(tela, ola_mundo, produto_vetor_escalar(VETOR_CIMA, 6));


    // Criando um objeto de texto um pouco menor
    Objeto* graficos_txt = criar_objeto_de_texto(3, 2, "Graficos!");
    centralizar_objeto(graficos_txt);
    mover_objeto(tela, graficos_txt, produto_vetor_escalar(VETOR_BAIXO, 2));

    int numero = 4;
    char texto[] = "Jogos";
    Objeto* jogos_txt = criar_objeto_de_texto(1, 1, "%s %d", texto, numero);
    centralizar_objeto(jogos_txt);
    trocar_cor_texto(jogos_txt, COLOR_VERDE);
    mover_objeto(tela, jogos_txt, produto_vetor_escalar(VETOR_BAIXO, 10));

    // 4. Renderização
    render(tela, true);
    
    printf("Pressione qualquer coisa para sair...\n");
    getchar();
    excluir_objeto(ola_mundo);
    excluir_objeto(graficos_txt);
    excluir_tela(tela);
    return 0;
}