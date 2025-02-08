# Kosmic Engine

Bem-vindo ao repositório da **Kosmic Engine**! Uma game-engine 3D feita em C++23 e OpenGL 3.3.

## Localização
[English](../../README.md)

## Estrutura do Projeto

- **Docs:**  
    Contém a documentação a Kosmic.

- **Engine:**  
    Possui o código da engine, como o core.

- **Sandbox:**  
    Utilizado para testar novas features e identificar bugs.

- **Thirdparty:**  
    Contém bibliotecas externas necessárias para o funcionamento da engine. Essas dependências podem ser compiladas ou integradas diretamente.

## Features Atuais

- **Sistema de Renderização:**  
    Suporta renderização com foco em 3D, com suporte a shaders e iluminação básica.

- **Gerenciamento de Recursos:**  
    Sistema de carregamento e gerenciamento de assets 3D utilizando a biblioteca `assimp`

## Como Compilar a Kosmic

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
     make -$(nproc)
     ```

Caso precise personalizar a compilação (por exemplo, selecionando o modo de build Debug ou Release), passe as opções adequadas ao comando `cmake`, por exemplo:

```
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Contribuindo

Sinta-se à vontade para criar issues ou enviar um pull requests com melhorias ou correções.  

Enjoy coding! :)
