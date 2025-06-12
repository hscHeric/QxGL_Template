#include "../lib/qxgl/src/qxgl.hpp"

#include <GL/glut.h>
#include <iostream>
#include <vector>

qxgl::Texture     myTexture;
qxgl::OrbitCamera myCamera;

// Dimensões da janela
int windowWidth  = 800;
int windowHeight = 600;

// Variáveis de controle de teclas (estado das teclas pressionadas)
bool keyW = false, keyA = false, keyS = false, keyD = false;
bool keyQ = false, keyE = false;

// Função para desenhar um chão quadriculado sem textura
void drawCheckeredFloor() {
  glDisable( GL_TEXTURE_2D );
  float floorSize  = 20.0f;
  int   numSquares = 20;
  float squareSize = floorSize / numSquares;

  glBegin( GL_QUADS );
  for ( int i = 0; i < numSquares; ++i ) {
    for ( int j = 0; j < numSquares; ++j ) {
      if ( ( i + j ) % 2 == 0 ) {
        glColor3f( 0.5f, 0.5f, 0.5f );
      } else {
        glColor3f( 0.3f, 0.3f, 0.3f );
      }

      float x1 = -floorSize / 2.0f + i * squareSize;
      float z1 = -floorSize / 2.0f + j * squareSize;
      float x2 = x1 + squareSize;
      float z2 = z1 + squareSize;

      glVertex3f( x1, 0.0f, z1 );
      glVertex3f( x2, 0.0f, z1 );
      glVertex3f( x2, 0.0f, z2 );
      glVertex3f( x1, 0.0f, z2 );
    }
  }
  glEnd();
  glEnable( GL_TEXTURE_2D );
}

// Função para desenhar uma pirâmide com textura
void drawTexturedPyramid() {
  if ( myTexture.isLoaded() ) {
    myTexture.bind();
  } else {
    glColor3f( 1.0f, 0.0f, 1.0f );
  }

  glBegin( GL_TRIANGLES );
  // Base da pirâmide (assumindo que a base está em y=0)
  // Triângulo 1 da base
  glNormal3f( 0.0f, -1.0f, 0.0f );
  glTexCoord2f( 0.0f, 0.0f );
  glVertex3f( -1.0f, 0.0f, 1.0f );
  glTexCoord2f( 1.0f, 0.0f );
  glVertex3f( 1.0f, 0.0f, 1.0f );
  glTexCoord2f( 1.0f, 1.0f );
  glVertex3f( 1.0f, 0.0f, -1.0f );

  // Triângulo 2 da base
  glNormal3f( 0.0f, -1.0f, 0.0f );
  glTexCoord2f( 0.0f, 0.0f );
  glVertex3f( -1.0f, 0.0f, 1.0f );
  glTexCoord2f( 1.0f, 1.0f );
  glVertex3f( 1.0f, 0.0f, -1.0f );
  glTexCoord2f( 0.0f, 1.0f );
  glVertex3f( -1.0f, 0.0f, -1.0f );

  // Faces laterais
  // Face Frontal (Z positivo)
  glNormal3f( 0.0f, 0.707f, 0.707f );
  glTexCoord2f( 0.5f, 1.0f );
  glVertex3f( 0.0f, 2.0f, 0.0f );
  glTexCoord2f( 0.0f, 0.0f );
  glVertex3f( -1.0f, 0.0f, 1.0f );
  glTexCoord2f( 1.0f, 0.0f );
  glVertex3f( 1.0f, 0.0f, 1.0f );

  // Face Direita (X positivo)
  glNormal3f( 0.707f, 0.707f, 0.0f );
  glTexCoord2f( 0.5f, 1.0f );
  glVertex3f( 0.0f, 2.0f, 0.0f );
  glTexCoord2f( 0.0f, 0.0f );
  glVertex3f( 1.0f, 0.0f, 1.0f );
  glTexCoord2f( 1.0f, 0.0f );
  glVertex3f( 1.0f, 0.0f, -1.0f );

  // Face Traseira (Z negativo)
  glNormal3f( 0.0f, 0.707f, -0.707f );
  glTexCoord2f( 0.5f, 1.0f );
  glVertex3f( 0.0f, 2.0f, 0.0f );
  glTexCoord2f( 0.0f, 0.0f );
  glVertex3f( 1.0f, 0.0f, -1.0f );
  glTexCoord2f( 1.0f, 0.0f );
  glVertex3f( -1.0f, 0.0f, -1.0f );

  // Face Esquerda (X negativo)
  glNormal3f( -0.707f, 0.707f, 0.0f );
  glTexCoord2f( 0.5f, 1.0f );
  glVertex3f( 0.0f, 2.0f, 0.0f );
  glTexCoord2f( 0.0f, 0.0f );
  glVertex3f( -1.0f, 0.0f, -1.0f );
  glTexCoord2f( 1.0f, 0.0f );
  glVertex3f( -1.0f, 0.0f, 1.0f );

  glEnd();

  if ( myTexture.isLoaded() ) {
    qxgl::Texture::unbind();
  }
}

// Função para testar todas as operações de OrbitCamera
void testOrbitCameraOperations() {
  std::cout << "\n--- Testando OrbitCamera ---" << std::endl;

  qxgl::OrbitCamera testCamera;  // Câmera default
  std::cout << "Câmera inicial: Eye=" << testCamera.eye.x << "," << testCamera.eye.y << ","
            << testCamera.eye.z << " Center=" << testCamera.center.x << "," << testCamera.center.y
            << "," << testCamera.center.z << " Up=" << testCamera.up.x << "," << testCamera.up.y
            << "," << testCamera.up.z << std::endl;

  // Testar zoom
  testCamera.zoom( 5.0f, 1.0f );
  std::cout << "Após zoom in (delta=5): Eye=" << testCamera.eye.x << "," << testCamera.eye.y << ","
            << testCamera.eye.z << std::endl;
  testCamera.zoom( -2.0f, 1.0f );
  std::cout << "Após zoom out (delta=-2): Eye=" << testCamera.eye.x << "," << testCamera.eye.y
            << "," << testCamera.eye.z << std::endl;

  // Resetar câmera para testes de translação e rotação
  testCamera = qxgl::OrbitCamera( qxgl::Vector3( 0.0F, 0.0F, 5.0F ),
                                  qxgl::Vector3( 0.0F, 0.0F, 0.0F ),
                                  qxgl::Vector3( 0.0F, 1.0F, 0.0F ) );

  // Testar translateX
  testCamera.translateX( 1.0f, 1.0f );
  std::cout << "Após translateX (delta=1): Eye=" << testCamera.eye.x << "," << testCamera.eye.y
            << "," << testCamera.eye.z << " Center=" << testCamera.center.x << ","
            << testCamera.center.y << "," << testCamera.center.z << std::endl;

  // Testar translateY
  testCamera.translateY( 1.0f, 1.0f );
  std::cout << "Após translateY (delta=1): Eye=" << testCamera.eye.x << "," << testCamera.eye.y
            << "," << testCamera.eye.z << " Center=" << testCamera.center.x << ","
            << testCamera.center.y << "," << testCamera.center.z << std::endl;

  // Resetar câmera para testes de rotação
  testCamera = qxgl::OrbitCamera( qxgl::Vector3( 0.0F, 0.0F, 5.0F ),
                                  qxgl::Vector3( 0.0F, 0.0F, 0.0F ),
                                  qxgl::Vector3( 0.0F, 1.0F, 0.0F ) );

  // Testar rotateX
  testCamera.rotateX( 45.0f, 1.0f );  // Rotação de 45 graus
  std::cout << "Após rotateX (angle=45): Eye=" << testCamera.eye.x << "," << testCamera.eye.y << ","
            << testCamera.eye.z << " Up=" << testCamera.up.x << "," << testCamera.up.y << ","
            << testCamera.up.z << std::endl;

  // Resetar câmera para testes de rotação
  testCamera = qxgl::OrbitCamera( qxgl::Vector3( 0.0F, 0.0F, 5.0F ),
                                  qxgl::Vector3( 0.0F, 0.0F, 0.0F ),
                                  qxgl::Vector3( 0.0F, 1.0F, 0.0F ) );

  // Testar rotateY
  testCamera.rotateY( 45.0f, 1.0f );  // Rotação de 45 graus
  std::cout << "Após rotateY (angle=45): Eye=" << testCamera.eye.x << "," << testCamera.eye.y << ","
            << testCamera.eye.z << " Up=" << testCamera.up.x << "," << testCamera.up.y << ","
            << testCamera.up.z << std::endl;

  // Resetar câmera para testes de rotação
  testCamera = qxgl::OrbitCamera( qxgl::Vector3( 0.0F, 0.0F, 5.0F ),
                                  qxgl::Vector3( 0.0F, 0.0F, 0.0F ),
                                  qxgl::Vector3( 0.0F, 1.0F, 0.0F ) );

  // Testar rotateZ
  testCamera.rotateZ( 45.0f, 1.0f );  // Rotação de 45 graus
  std::cout << "Após rotateZ (angle=45): Eye=" << testCamera.eye.x << "," << testCamera.eye.y << ","
            << testCamera.eye.z << " Up=" << testCamera.up.x << "," << testCamera.up.y << ","
            << testCamera.up.z << std::endl;

  // Testar getPickedPoint (exemplo de uso)
  qxgl::Vector3 pickedPoint = testCamera.getPickedPoint( windowWidth / 2.0f, windowHeight / 2.0f );
  std::cout << "Ponto 'selecionado' no centro da tela (exemplo): " << pickedPoint.x << ","
            << pickedPoint.y << "," << pickedPoint.z << std::endl;
}

// Função para testar todas as operações de Vector3
void testVector3Operations() {
  std::cout << "\n--- Testando Vector3 ---" << std::endl;

  qxgl::Vector3 v1( 1.0f, 2.0f, 3.0f );
  qxgl::Vector3 v2( 4.0f, 5.0f, 6.0f );
  qxgl::Vector3 v3;

  std::cout << "v1: " << v1.x << "," << v1.y << "," << v1.z << std::endl;
  std::cout << "v2: " << v2.x << "," << v2.y << "," << v2.z << std::endl;
  std::cout << "v3 (padrão): " << v3.x << "," << v3.y << "," << v3.z << std::endl;

  // length() e lengthSquared()
  std::cout << "Comprimento de v1: " << v1.length() << std::endl;
  std::cout << "Comprimento ao quadrado de v1: " << v1.lengthSquared() << std::endl;

  // normalize() e unit()
  qxgl::Vector3 v1_normalized = v1;
  v1_normalized.normalize();
  std::cout << "v1 normalizado (in place): " << v1_normalized.x << "," << v1_normalized.y << ","
            << v1_normalized.z << std::endl;
  qxgl::Vector3 v1_unit = v1.unit();
  std::cout << "Vetor unitário de v1: " << v1_unit.x << "," << v1_unit.y << "," << v1_unit.z
            << std::endl;

  // projectOnto()
  qxgl::Vector3 proj = v1.projectOnto( v2 );
  std::cout << "v1 projetado em v2: " << proj.x << "," << proj.y << "," << proj.z << std::endl;

  // assign()
  v3.assign( v1 );
  std::cout << "v3 após assign(v1): " << v3.x << "," << v3.y << "," << v3.z << std::endl;

  // add() e addInPlace()
  qxgl::Vector3 v_sum = v1.add( v2 );
  std::cout << "v1 + v2 (usando add): " << v_sum.x << "," << v_sum.y << "," << v_sum.z << std::endl;
  qxgl::Vector3 v1_copy = v1;
  v1_copy.addInPlace( v2 );
  std::cout << "v1 após addInPlace(v2): " << v1_copy.x << "," << v1_copy.y << "," << v1_copy.z
            << std::endl;

  // subtract()
  qxgl::Vector3 v_diff = v1.subtract( v2 );
  std::cout << "v1 - v2 (usando subtract): " << v_diff.x << "," << v_diff.y << "," << v_diff.z
            << std::endl;

  // multiply()
  qxgl::Vector3 v_scaled = v1.multiply( 2.0f );
  std::cout << "v1 * 2.0 (usando multiply): " << v_scaled.x << "," << v_scaled.y << ","
            << v_scaled.z << std::endl;

  // distanceTo()
  std::cout << "Distância de v1 a v2: " << v1.distanceTo( v2 ) << std::endl;

  // cross()
  qxgl::Vector3 v_cross = v1.cross( v2 );
  std::cout << "Produto vetorial de v1 e v2: " << v_cross.x << "," << v_cross.y << "," << v_cross.z
            << std::endl;

  // dot()
  std::cout << "Produto escalar de v1 e v2: " << v1.dot( v2 ) << std::endl;

  // Operadores sobrecarregados
  qxgl::Vector3 op_add = v1 + v2;
  std::cout << "v1 + v2 (operador+): " << op_add.x << "," << op_add.y << "," << op_add.z
            << std::endl;

  qxgl::Vector3 op_sub = v1 - v2;
  std::cout << "v1 - v2 (operador-): " << op_sub.x << "," << op_sub.y << "," << op_sub.z
            << std::endl;

  qxgl::Vector3 op_mul_scalar = v1 * 3.0f;
  std::cout << "v1 * 3.0 (operador* escalar): " << op_mul_scalar.x << "," << op_mul_scalar.y << ","
            << op_mul_scalar.z << std::endl;

  float op_dot = v1 * v2;
  std::cout << "v1 . v2 (operador* produto escalar): " << op_dot << std::endl;

  qxgl::Vector3 op_cross = v1 ^ v2;
  std::cout << "v1 x v2 (operador^ produto vetorial): " << op_cross.x << "," << op_cross.y << ","
            << op_cross.z << std::endl;

  qxgl::Vector3 op_unit = !v1;
  std::cout << "Vetor unitário de v1 (operador!): " << op_unit.x << "," << op_unit.y << ","
            << op_unit.z << std::endl;
}

// Função para atualizar a câmera baseada nas teclas pressionadas
void updateCameraFromKeys() {
  GLfloat moveSpeed   = 0.1f;
  GLfloat rotateSpeed = 0.5f;

  if ( keyW ) {  // Move a câmera para frente (zoom in)
    myCamera.zoom( moveSpeed * 10.0f, 1.0f );
  }
  if ( keyS ) {  // Move a câmera para trás (zoom out)
    myCamera.zoom( -moveSpeed * 10.0f, 1.0f );
  }
  if ( keyA ) {  // Move a câmera para a esquerda (pan X)
    myCamera.translateX( moveSpeed * 50.0f, 0.01f );
  }
  if ( keyD ) {  // Move a câmera para a direita (pan X)
    myCamera.translateX( -moveSpeed * 50.0f, 0.01f );
  }
  if ( keyQ ) {  // Rotação Z (roll) para a esquerda
    myCamera.rotateZ( rotateSpeed * 10.0f, 0.1f );
  }
  if ( keyE ) {  // Rotação Z (roll) para a direita
    myCamera.rotateZ( -rotateSpeed * 10.0f, 0.1f );
  }
  glutPostRedisplay();  // Redesenha a cena após o movimento
}

// Função para lidar com o pressionar de teclas
void keyboard( unsigned char key, int x, int y ) {
  switch ( key ) {
    case 'w':
    case 'W': keyW = true; break;
    case 'a':
    case 'A': keyA = true; break;
    case 's':
    case 'S': keyS = true; break;
    case 'd':
    case 'D': keyD = true; break;
    case 'q':
    case 'Q': keyQ = true; break;
    case 'e':
    case 'E': keyE = true; break;
    case 27:  // Tecla ESC para sair
      exit( 0 );
      break;
  }
}

// Função para lidar com o soltar de teclas
void keyboardUp( unsigned char key, int x, int y ) {
  switch ( key ) {
    case 'w':
    case 'W': keyW = false; break;
    case 'a':
    case 'A': keyA = false; break;
    case 's':
    case 'S': keyS = false; break;
    case 'd':
    case 'D': keyD = false; break;
    case 'q':
    case 'Q': keyQ = false; break;
    case 'e':
    case 'E': keyE = false; break;
  }
}

// Função de idle para atualizar a câmera continuamente quando as teclas estiverem pressionadas
void idle() {
  updateCameraFromKeys();
}

void init() {
  glClearColor( 0.2f, 0.2f, 0.2f, 1.0f );
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_TEXTURE_2D );

  if ( !myTexture.load( "../assets/textures/placeholder.png" ) ) {
    std::cerr << "Erro ao carregar a textura placeholder.png" << std::endl;
  } else {
    std::cout << "Textura placeholder.png carregada com sucesso! ID: " << myTexture.getTextureID()
              << std::endl;
  }

  myCamera = qxgl::OrbitCamera( qxgl::Vector3( 0.0F, 5.0F, 15.0F ),
                                qxgl::Vector3( 0.0F, 1.0F, 0.0F ),
                                qxgl::Vector3( 0.0F, 1.0F, 0.0F ) );

  // Explicar as instruções de uso da câmera
  std::cout << "\n--- Instruções de Uso da Câmera ---" << std::endl;
  std::cout << " - Mover para frente (Zoom In): Tecla 'W'" << std::endl;
  std::cout << " - Mover para trás (Zoom Out): Tecla 'S'" << std::endl;
  std::cout << " - Mover para a esquerda (Pan X): Tecla 'A'" << std::endl;
  std::cout << " - Mover para a direita (Pan X): Tecla 'D'" << std::endl;
  std::cout << " - Rotacionar para a esquerda (Roll Z): Tecla 'Q'" << std::endl;
  std::cout << " - Rotacionar para a direita (Roll Z): Tecla 'E'" << std::endl;
  std::cout
    << " - Girar a câmera (Orbitar): Mantenha o Botão Esquerdo do mouse pressionado e arraste."
    << std::endl;
  std::cout
    << " - Mover a câmera lateralmente (Pan Y e X): Mantenha o Botão Direito do mouse pressionado e arraste."
    << std::endl;
  std::cout
    << " - Zoom com a Roda do Mouse: Role para cima para zoom in, role para baixo para zoom out."
    << std::endl;
  std::cout << " - Pressione ESC para sair." << std::endl;

  // Executar os testes de Vector3 e OrbitCamera na inicialização
  testVector3Operations();
  testOrbitCameraOperations();
}

void display() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.0f );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  gluLookAt( myCamera.eye.x,
             myCamera.eye.y,
             myCamera.eye.z,
             myCamera.center.x,
             myCamera.center.y,
             myCamera.center.z,
             myCamera.up.x,
             myCamera.up.y,
             myCamera.up.z );

  // Desenha o chão quadriculado
  drawCheckeredFloor();

  // Desenha a pirâmide texturizada
  glPushMatrix();
  glTranslatef( 0.0f, 0.0f, 0.0f );
  drawTexturedPyramid();
  glPopMatrix();

  glutSwapBuffers();
}

void reshape( int w, int h ) {
  windowWidth  = w;
  windowHeight = h;
  glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
  glutPostRedisplay();
}

// Variáveis para controle do mouse
int  lastMouseX, lastMouseY;
bool isRotating = false;
bool isPanning  = false;

void mouse( int button, int state, int x, int y ) {
  if ( button == GLUT_LEFT_BUTTON ) {
    if ( state == GLUT_DOWN ) {
      isRotating = true;
      lastMouseX = x;
      lastMouseY = y;
    } else {
      isRotating = false;
    }
  } else if ( button == GLUT_RIGHT_BUTTON ) {
    if ( state == GLUT_DOWN ) {
      isPanning  = true;
      lastMouseX = x;
      lastMouseY = y;
    } else {
      isPanning = false;
    }
  } else if ( button == 3 ) {  // Scroll up
    myCamera.zoom( 1.0f, 0.1f );
    glutPostRedisplay();
  } else if ( button == 4 ) {  // Scroll down
    myCamera.zoom( -1.0f, 0.1f );
    glutPostRedisplay();
  }
}

void motion( int x, int y ) {
  if ( isRotating ) {
    GLfloat deltaX = (GLfloat)( x - lastMouseX );
    GLfloat deltaY = (GLfloat)( y - lastMouseY );

    myCamera.rotateY( deltaX, 0.5f );  // Rotação horizontal
    myCamera.rotateX( deltaY, 0.5f );  // Rotação vertical

    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
  } else if ( isPanning ) {
    GLfloat deltaX = (GLfloat)( x - lastMouseX );
    GLfloat deltaY = (GLfloat)( y - lastMouseY );

    myCamera.translateX( -deltaX, 0.01f );  // Pan para a esquerda/direita
    myCamera.translateY( deltaY, 0.01f );   // Pan para cima/baixo

    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
  }
}

int main( int argc, char **argv ) {
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
  glutInitWindowSize( windowWidth, windowHeight );
  glutCreateWindow( "Teste de Textura e Câmera qxgl" );

  init();

  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutMouseFunc( mouse );
  glutMotionFunc( motion );
  glutKeyboardFunc( keyboard );      // Registra a função para teclas pressionadas
  glutKeyboardUpFunc( keyboardUp );  // Registra a função para teclas soltas
  glutIdleFunc( idle );              // Registra a função idle para atualização contínua

  glutMainLoop();

  return 0;
}
