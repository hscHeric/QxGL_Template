#include "../lib/qxgl/src/qxgl.hpp"

#include <GL/glut.h>
#include <iostream>
#include <map>  // For key state management

// Variáveis globais para a câmera e o estado do mouse
qxgl::OrbitCamera cam;
int               last_mouse_x, last_mouse_y;
bool              is_rotating = false;
bool              is_panning  = false;

// Texturas globais
qxgl::Texture myTexture;
qxgl::Texture floorTexture;  // Nova textura para o chão

// Estado das teclas para movimento WASD
std::map<unsigned char, bool> keyStates;

// Função de inicialização do OpenGL
void initGL() {
  glClearColor( 0.2F, 0.2F, 0.2F, 1.0F );  // Cor de fundo
  glEnable( GL_DEPTH_TEST );               // Habilita o teste de profundidade
  glEnable( GL_TEXTURE_2D );               // Habilita texturas 2D
  glShadeModel( GL_SMOOTH );               // Para um sombreamento mais suave se houver luz

  // Configuração de iluminação básica
  glEnable( GL_LIGHTING );  // Habilita o sistema de iluminação
  glEnable( GL_LIGHT0 );    // Habilita a luz 0

  // Definindo a posição da luz (luz direcional vindo da frente e acima)
  GLfloat light_position[] = { 1.0F, 1.0F, 1.0F, 0.0F };  // w = 0.0 para luz direcional
  glLightfv( GL_LIGHT0, GL_POSITION, light_position );

  // Definindo as propriedades da luz
  GLfloat light_ambient[]  = { 0.5F, 0.5F, 0.5F, 1.0F };
  GLfloat light_diffuse[]  = { 0.8F, 0.8F, 0.8F, 1.0F };
  GLfloat light_specular[] = { 1.0F, 1.0F, 1.0F, 1.0F };
  glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
  glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
  glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );

  // Habilitando o rastreamento de cores para que glColor funcione com iluminação
  glEnable( GL_COLOR_MATERIAL );
  glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
}

// Função para desenhar o chão
void drawFloor() {
  // Definir as coordenadas do plano do chão
  float floorSize = 10.0f;  // Tamanho do chão
  float floorY    = -1.0f;  // Posição Y do chão (abaixo do cubo)

  // Vincula a textura do chão
  floorTexture.bind();
  glColor3f( 1.0f, 1.0f, 1.0f );  // Cor branca para a textura não ser tingida

  glBegin( GL_QUADS );
  glNormal3f( 0.0f, 1.0f, 0.0f );  // Normal apontando para cima

  // Canto inferior esquerdo
  glTexCoord2f( 0.0f, 0.0f );
  glVertex3f( -floorSize, floorY, -floorSize );

  // Canto inferior direito
  glTexCoord2f( 1.0f, 0.0f );
  glVertex3f( floorSize, floorY, -floorSize );

  // Canto superior direito
  glTexCoord2f( 1.0f, 1.0f );
  glVertex3f( floorSize, floorY, floorSize );

  // Canto superior esquerdo
  glTexCoord2f( 0.0f, 1.0f );
  glVertex3f( -floorSize, floorY, floorSize );
  glEnd();

  floorTexture.unbind();  // Desvincula a textura do chão
}

// Função para desenhar a cena
void display() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();

  // Configura a câmera
  gluLookAt( cam.eye.x,
             cam.eye.y,
             cam.eye.z,
             cam.center.x,
             cam.center.y,
             cam.center.z,
             cam.up.x,
             cam.up.y,
             cam.up.z );

  // Desenha o chão
  drawFloor();

  // Desenha um cubo com textura
  // Vincula a textura antes de desenhar
  myTexture.bind();

  glColor3f( 1.0F, 1.0F, 1.0F );  // Cor branca para não tingir a textura

  // Começa a desenhar o cubo
  glBegin( GL_QUADS );

  // Face frontal (Z positivo)
  glNormal3f( 0.0F, 0.0F, 1.0F );  // Normal da face frontal
  glTexCoord2f( 0.0F, 0.0F );
  glVertex3f( -1.0F, -1.0F, 1.0F );
  glTexCoord2f( 1.0F, 0.0F );
  glVertex3f( 1.0F, -1.0F, 1.0F );
  glTexCoord2f( 1.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, 1.0F );
  glTexCoord2f( 0.0F, 1.0F );
  glVertex3f( -1.0F, 1.0F, 1.0F );

  // Face traseira (Z negativo)
  glNormal3f( 0.0F, 0.0F, -1.0F );  // Normal da face traseira
  glTexCoord2f( 1.0F, 0.0F );
  glVertex3f( -1.0F, -1.0F, -1.0F );
  glTexCoord2f( 1.0F, 1.0F );
  glVertex3f( -1.0F, 1.0F, -1.0F );
  glTexCoord2f( 0.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, -1.0F );
  glTexCoord2f( 0.0F, 0.0F );
  glVertex3f( 1.0F, -1.0F, -1.0F );

  // Face superior (Y positivo)
  glNormal3f( 0.0F, 1.0F, 0.0F );  // Normal da face superior
  glTexCoord2f( 0.0F, 1.0F );
  glVertex3f( -1.0F, 1.0F, -1.0F );
  glTexCoord2f( 0.0F, 0.0F );
  glVertex3f( -1.0F, 1.0F, 1.0F );
  glTexCoord2f( 1.0F, 0.0F );
  glVertex3f( 1.0F, 1.0F, 1.0F );
  glTexCoord2f( 1.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, -1.0F );

  // Face inferior (Y negativo)
  glNormal3f( 0.0F, -1.0F, 0.0F );  // Normal da face inferior
  glTexCoord2f( 1.0F, 1.0F );
  glVertex3f( -1.0F, -1.0F, -1.0F );
  glTexCoord2f( 0.0F, 1.0F );
  glVertex3f( 1.0F, -1.0F, -1.0F );
  glTexCoord2f( 0.0F, 0.0F );
  glVertex3f( 1.0F, -1.0F, 1.0F );
  glTexCoord2f( 1.0F, 0.0F );
  glVertex3f( -1.0F, -1.0F, 1.0F );

  // Face direita (X positivo)
  glNormal3f( 1.0F, 0.0F, 0.0F );  // Normal da face direita
  glTexCoord2f( 1.0F, 0.0F );
  glVertex3f( 1.0F, -1.0F, -1.0F );
  glTexCoord2f( 1.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, -1.0F );
  glTexCoord2f( 0.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, 1.0F );
  glTexCoord2f( 0.0F, 0.0F );
  glVertex3f( 1.0F, -1.0F, 1.0F );

  // Face esquerda (X negativo)
  glNormal3f( -1.0F, 0.0F, 0.0F );  // Normal da face esquerda
  glTexCoord2f( 0.0F, 0.0F );
  glVertex3f( -1.0F, -1.0F, -1.0F );
  glTexCoord2f( 1.0F, 0.0F );
  glVertex3f( -1.0F, -1.0F, 1.0F );
  glTexCoord2f( 1.0F, 1.0F );
  glVertex3f( -1.0F, 1.0F, 1.0F );
  glTexCoord2f( 0.0F, 1.0F );
  glVertex3f( -1.0F, 1.0F, -1.0F );

  glEnd();

  // Desvincula a textura após o desenho
  myTexture.unbind();

  glutSwapBuffers();
}

// Função de redimensionamento da janela
void reshape( int w, int h ) {
  glViewport( 0, 0, w, h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 45.0, (double)w / (double)h, 0.1, 100.0 );
  glMatrixMode( GL_MODELVIEW );
}

// Função de callback do mouse
void mouse( int button, int state, int x, int y ) {
  if ( button == GLUT_LEFT_BUTTON ) {
    if ( state == GLUT_DOWN ) {
      is_rotating  = true;
      last_mouse_x = x;
      last_mouse_y = y;
    } else {
      is_rotating = false;
    }
  }
  // Middle button for panning (remapped to right click for convenience on many systems)
  // You can change this to GLUT_MIDDLE_BUTTON if you have a middle mouse button
  if ( button == GLUT_RIGHT_BUTTON ) {  // Using GLUT_RIGHT_BUTTON for panning
    if ( state == GLUT_DOWN ) {
      is_panning   = true;
      last_mouse_x = x;
      last_mouse_y = y;
    } else {
      is_panning = false;
    }
  }
  if ( button == 3 ) {        // Scroll up
    cam.zoom( 0.5F, 10.0F );  // Smaller delta for smoother zoom
    glutPostRedisplay();
  }
  if ( button == 4 ) {         // Scroll down
    cam.zoom( -0.5F, 10.0F );  // Smaller delta for smoother zoom
    glutPostRedisplay();
  }
}

// Função de movimento do mouse
void motion( int x, int y ) {
  if ( is_rotating ) {
    auto deltaX  = (float)( x - last_mouse_x );
    auto deltaY  = (float)( y - last_mouse_y );
    last_mouse_x = x;
    last_mouse_y = y;

    cam.rotateY( -deltaX * 0.2F, 1.0F );  // Reduced sensitivity for rotation
    cam.rotateX( deltaY * 0.2F, 1.0F );   // Reduced sensitivity for rotation
    glutPostRedisplay();
  }
  if ( is_panning ) {
    auto deltaX  = (float)( x - last_mouse_x );
    auto deltaY  = (float)( y - last_mouse_y );
    last_mouse_x = x;
    last_mouse_y = y;

    // Adjust pan sensitivity based on camera distance
    float pan_sensitivity = cam.eye.distanceTo( cam.center ) * 0.005f;  // Scale with distance
    cam.translateX( deltaX * pan_sensitivity, 1.0F );
    cam.translateY( -deltaY * pan_sensitivity, 1.0F );  // Invert Y for screen coordinates
    glutPostRedisplay();
  }
}

// Função de callback de teclado (pressionar)
void keyboard( unsigned char key, int x, int y ) {
  keyStates[key] = true;
}

// Função de callback de teclado (soltar)
void keyboardUp( unsigned char key, int x, int y ) {
  keyStates[key] = false;
}

// Função de atualização para movimento contínuo (WASD)
void update( int value ) {
  float move_speed = 0.05F;  // Adjust this for desired WASD speed
  float zoom_speed = 0.1F;   // Adjust this for desired WASD zoom speed

  if ( keyStates['w'] || keyStates['W'] ) {
    cam.zoom( zoom_speed, 1.0F );
  }
  if ( keyStates['s'] || keyStates['S'] ) {
    cam.zoom( -zoom_speed, 1.0F );
  }
  if ( keyStates['a'] || keyStates['A'] ) {
    cam.translateX( -move_speed, 1.0F );
  }
  if ( keyStates['d'] || keyStates['D'] ) {
    cam.translateX( move_speed, 1.0F );
  }

  if ( keyStates['q'] || keyStates['Q'] ) {  // Rotate Z left
    cam.rotateZ( -0.5F, 1.0F );
  }
  if ( keyStates['e'] || keyStates['E'] ) {  // Rotate Z right
    cam.rotateZ( 0.5F, 1.0F );
  }
  glutPostRedisplay();
  glutTimerFunc( 16, update, 0 );  // ~60 FPS update rate
}

int main( int argc, char **argv ) {
  // Testando a classe Vector3
  std::cout << "--- Testando Vector3 ---" << std::endl;
  qxgl::Vector3 v1( 1.0F, 2.0F, 3.0F );
  qxgl::Vector3 v2( 4.0F, 5.0F, 6.0F );

  std::cout << "V1: (" << v1.x << ", " << v1.y << ", " << v1.z << ")" << std::endl;
  std::cout << "V2: (" << v2.x << ", " << v2.y << ", " << v2.z << ")" << std::endl;

  qxgl::Vector3 sum = v1 + v2;
  std::cout << "V1 + V2: (" << sum.x << ", " << sum.y << ", " << sum.z << ")" << std::endl;

  float dot_prod = v1 * v2;
  std::cout << "V1 . V2 (produto escalar): " << dot_prod << std::endl;

  qxgl::Vector3 cross_prod = v1 ^ v2;
  std::cout << "V1 x V2 (produto vetorial): (" << cross_prod.x << ", " << cross_prod.y << ", "
            << cross_prod.z << ")" << std::endl;

  std::cout << "Comprimento de V1: " << v1.length() << std::endl;
  std::cout << "Vetor unitário de V1: (" << ( !v1 ).x << ", " << ( !v1 ).y << ", " << ( !v1 ).z
            << ")" << std::endl;
  std::cout << std::endl;

  // Testando a classe OrbitCamera (a visualização abaixo usará essa)
  std::cout << "--- Testando OrbitCamera (interação visual) ---" << std::endl;
  std::cout << "Mova o mouse com o botão esquerdo pressionado para rotacionar." << std::endl;
  std::cout << "Mova o mouse com o botão direito pressionado para transladar (pan)." << std::endl;
  std::cout << "Use o scroll do mouse ou W/S para zoom." << std::endl;
  std::cout << "Use A/D para transladar horizontalmente." << std::endl;
  std::cout << "Use Q/E para rotacionar em torno do eixo Z (roll)." << std::endl;
  std::cout << std::endl;

  // Carregamento de texturas
  myTexture.load( "../assets/textures/placeholder.png" );  // Carrega a textura do cubo
  if ( myTexture.isLoaded() ) {
    std::cout << "Textura 'placeholder.png' carregada com sucesso! ID: " << myTexture.getTextureID()
              << std::endl;
  } else {
    std::cerr
      << "Falha ao carregar a textura 'placeholder.png'. Certifique-se de que o arquivo existe no caminho correto."
      << std::endl;
  }

  // Carrega a textura do chão
  floorTexture.load( "../assets/textures/placeholder.png" );
  if ( floorTexture.isLoaded() ) {
    std::cout << "Textura 'floor.png' carregada com sucesso! ID: " << floorTexture.getTextureID()
              << std::endl;
  } else {
    std::cerr
      << "Falha ao carregar a textura 'floor.png'. Crie uma imagem 'floor.png' ou ajuste o caminho."
      << std::endl;
  }

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize( 800, 600 );
  glutCreateWindow( "QxGL Camera Test" );

  initGL();

  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutMouseFunc( mouse );
  glutMotionFunc( motion );
  glutKeyboardFunc( keyboard );      // Registra a função de teclado
  glutKeyboardUpFunc( keyboardUp );  // Registra a função de soltar tecla
  glutTimerFunc( 0, update, 0 );     // Inicia o loop de atualização

  glutMainLoop();

  return 0;
}
