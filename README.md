# 🪐 Planetário

Projeto desenvolvido para a disciplina de **Computação Gráfica**, representando um esquema 3D simplificado do sistema solar usando **OpenGL e GLUT**.

## 🌌 Sobre o projeto

- Visualização em 3D dos planetas do sistema solar
- Movimento orbital básico simulando translação dos planetas
- Implementado em **C com GLUT**

## 🛠️ Tecnologias utilizadas

- C
- OpenGL
- GLUT (OpenGL Utility Toolkit)
- GLU (OpenGL Utility Library)
- Math.h

## 💻 Como compilar e executar

Certifique-se de ter as bibliotecas **OpenGL, GLUT e GLU** instaladas no sistema.

### ✅ Comando de compilação:

```bash
gcc planetario.c -lGL -lglut -lm -lGLU -o planetario && ./planetario
```

Se estiver usando Ubuntu/Debian e ainda não tiver as dependências, instale com:

```bash
sudo apt update
sudo apt install freeglut3-dev
```

## 📦 Arquivos

- `planetario.c` — Código-fonte principal do planetário

## 🧪 Observações

- O sistema solar não está em escala real.
- O foco é a simulação visual de órbitas e posições planetárias para fins educacionais.
  
---

> Este projeto foi criado com fins acadêmicos, para explorar conceitos de renderização e transformação em 3D com OpenGL.
```
