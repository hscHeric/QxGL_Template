<h1>QxGL_Template</h1>

<p>Este repositório contém um template básico para o projeto QxGL, configurado com suporte a CMake e submódulos.</p>

<h2>Como Baixar o Template</h2>
<p>Siga os passos abaixo para clonar o repositório com os submódulos:</p>

<h3>Passo 1: Clone o Repositório do template</h3>
<p>Primeiro, clone o repositório usando o comando abaixo (Observe que você deve alterar a url para a url do seu projeto):</p>
<pre><code>git clone --recursive https://github.com/hscHeric/QxGL_Template.git</code></pre>
<p>A opção <code>--recursive</code> é importante para garantir que todos os submódulos sejam clonados juntamente com o repositório principal.</p>

<h3>Passo 2: Atualize os Submódulos (Caso Não Use o <code>--recursive</code>)</h3>
<p>Se você esqueceu de usar a opção <code>--recursive</code>, pode atualizar os submódulos manualmente com o seguinte comando dentro do diretório do repositório clonado:</p>
<pre><code>git submodule update --init --recursive</code></pre>

<h3>Passo 3: Dependências</h3>
<p>Este projeto requer as seguintes dependências:</p>
<ul>
  <li><strong>GLU</strong>: Biblioteca utilitária para OpenGL</li>
  <li><strong>GLUT</strong>: Biblioteca para criar janelas e controlar entradas em OpenGL</li>
  <li><strong>OpenGL</strong>: API para gráficos 3D</li>
  <li><strong>C++20</strong>: O código foi desenvolvido utilizando a versão C++20, portanto, você deve garantir que seu compilador suporte essa versão.</li>
  <li><strong>CMake</strong>: O CMake deve estar instalado para configurar e compilar o projeto.</li>
</ul>

<h3>Passo 4: Compilando o Projeto</h3>
<p>Após clonar o repositório e inicializar os submódulos, você pode compilar o projeto com CMake. Execute os seguintes comandos:</p>

<ol>
  <li>Criar um diretório de construção:</li>
  <pre><code>mkdir build
cd build</code></pre>

  <li>Execute o CMake para configurar o projeto:</li>
  <pre><code>cmake ..</code></pre>

  <li>Compile o projeto:</li>
  <pre><code>make</code></pre>
</ol>

<p>Agora, o template estará pronto para ser usado!</p>

<h2>Mais Informações</h2>
<p>Consulte o arquivo <code>README.md</code> para detalhes adicionais sobre a estrutura do projeto e instruções de uso.</p>
