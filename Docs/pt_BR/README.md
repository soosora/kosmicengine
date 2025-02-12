# Kosmic Engine [![🐧 Linux Build 🐧](https://github.com/soosora/kosmicengine/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/soosora/kosmicengine/actions/workflows/cmake-multi-platform.yml)

Oiee! Bem-vindo(a) ao repositório da **Kosmic Engine**! Uma game-engine 3D feita em C++23 e OpenGL 3.3.

## Localização
[English](../../README.md)

## Estrutura do Projeto

- **Docs:**  
    Contém a documentação da Kosmic.

- **Engine:**  
    Possui o código da biblioteca principal da engine.

- **Examples:**  
    - Sandbox: Utilizado para testar novas features e identificar bugs.
    - Pong: Implementação simples do Pong.

- **Thirdparty:**  
    Contém bibliotecas externas necessárias para o funcionamento da engine.

## Features Atuais

- **Sistema de Renderização:**  
    Suporte a renderização com o foco em 3D, com iluminações básicas.

- **Gerenciamento de Recursos:**  
    Sistema de carregamento e gerenciamento de assets 3D utilizando a biblioteca `assimp`.

## Dependências

- assimp: Carregamento de assets 3D.
- SDL2: gerenciamento de janelas e input.
- glad: carregamento do OpenGL.
- glm: matemática.
- spdlog: logging.
- stb_image: carregamento de imagens.
- imgui: GUI.
- entt: ECS.

## Como Compilar a Kosmic

### Linux

Para compilar o projeto, siga as instruções abaixo:

1. **Crie um diretório de build** (dentro do diretório do projeto):

     ```
     mkdir Build && cd Build
     ```

2. **Baixe as bibliotecas externas**:

     ```
     git submodule update --init --recursive
     ```

3. **Gere os arquivos de compilação:**

     ```
     cmake ..
     ```

4. **Compile o projeto:**

     ```
     cmake --build .
     ```

     ou

     ```
     make -j$(nproc)
     ```

Caso precise personalizar a compilação (por exemplo, selecionando o modo de build Debug ou Release), passe as opções adequadas ao comando `cmake`:

```
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Contribuindo

Sinta-se à vontade para criar issues ou enviar um pull requests com melhorias ou correções.  

Se divirta! :)
