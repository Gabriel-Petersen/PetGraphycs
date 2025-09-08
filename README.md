\mainpage PetGraphycs - Biblioteca Gráfica para Terminal

Biblioteca para criação de gráficos e animações em terminal usando caracteres coloridos como pixeis.  
Compatível com sprites exportados do [Piskel](https://www.piskelapp.com/).

## Como Compilar

### 1. Estrutura do Projeto

\code{.unparsed}
MinhaLib/
├── include/
│   └── graphycs.h       // Cabeçalho público (modo profissional)
│   └── graphycsTxt.h    // Cabeçalho de textos - inclusão automática
│   └── graphycs_all.h   // Versão única (cabeçalho + implementação)
├── src/
│   └── graphycs.c       // Código-fonte da biblioteca
└── examples/            // Exemplos de uso
    ├── assets/          // Assets usados nos exemplos
    ├── 01_basico.c      // Exemplo: renderização estática
    ├── 02_piskel.c      // Exemplo: uso de sprites do Piskel
    └── 03_animacao.c    // Exemplo: animações com spritesheets
    └── 04_textos.c      // Exemplo: criação de textos para telas
\endcode

---

### 2. Modos de Uso

A biblioteca pode ser usada de **duas formas**:

#### 🔹 Modo Profissional (recomendado)

Separando cabeçalho e código-fonte. Mais adequado para projetos médios e grandes.

\code{.sh}
gcc -Wall -Wextra -Iinclude meu_programa.c src/graphycs.c -o meu_programa
\endcode

#### 🔹 Modo Prático (single-header)

Inclui tudo direto com `graphycs_all.h`.  
Ideal para iniciantes ou projetos pequenos com apenas 1 arquivo `.c`.

\code{.c}
#include "include/graphycs_all.h"

int main() 
{
    Screen* tela = criar_tela(new_Vector2(80, 25), COLOR_PRETO, 0);
    // seu código...
}
\endcode

Compilação fica simples, **sem precisar adicionar `src/graphycs.c`**:

\code{.sh}
gcc -Wall -Wextra meu_programa.c -o meu_programa
\endcode

Se seu Sistema Operacional é Linux/Posix, a primeira linha do seu código (antes de #include) deve ser:

\code{.c}
#define IN_LINUX_SO
\endcode

Se é Windows, não precisa modificar nada.
---

## Usando Sprites do Piskel

### 1. Alteração do Arquivo

- Exporte seu sprite no Piskel como arquivo C
- Converta para `.h` (edite diretamente o arquivo, sem duplicar):
  - Renomeie o arquivo para `meu_sprite.h`
  - Adicione no início:

\code{.c}
#ifndef MEU_SPRITE_H
#define MEU_SPRITE_H
\endcode

  - Adicione no final:

\code{.c}
#endif
\endcode

### 2. Uso no seu código

\code{.c}
#include "meu_sprite.h" // Use o caminho correto (ex: "assets/meu_sprite.h")
Objeto* sprite = criar_piskel_obj(sprite_data, SPRITE_WIDTH, SPRITE_HEIGHT);
\endcode

## Exemplos Práticos

| Arquivo         | Descrição                          | Comando de Compilação |
|-----------------|------------------------------------|----------------------------------------------------------------------------|
| `01_basico.c`   | Renderização estática de objetos   | `gcc -Iinclude examples/01_basico.c src/graphycs.c -o examples/basico`     |
| `02_piskel.c`   | Carregamento de sprites do Piskel  | `gcc -Iinclude examples/02_piskel.c src/graphycs.c -o examples/piskel`     |
| `03_animacao.c` | Spritesheets e ObjetoComplexo      | `gcc -Iinclude examples/03_animacao.c src/graphycs.c -o examples/animacao` |
| `04_textos.c`   | Criação de textos de tela          | `gcc -Iinclude examples/04_textos.c src/graphycs.c -o examples/textos`     |

Execute com:

\code{.sh}
./examples/basico.exe
./examples/piskel.exe
./examples/animacao.exe
./examples/textos.exe
\endcode

## Documentação

Acesse a documentação gerada pelo Doxygen:  
🔗 [https://gabriel-petersen.github.io/PetGraphycs/](https://gabriel-petersen.github.io/PetGraphycs/)