#include "../lib/qxgl/src/qxgl.hpp"

#include <iostream>
float g_pos_x  = 0.0F;
float g_pos_y  = 0.0F;
float g_radius = 0.0F;

qxgl::Transform g_transform;

// --- Definições de Objetos ---

void draw_house() {
  // A renderização com glBegin/glEnd é mantida como solicitado.
  // Face frontal
  glBegin( GL_POLYGON );
  glNormal3f( 0.0F, 0.0F, 1.0F );
  glVertex3f( 0.0F, 0.0F, 1.0F );
  glVertex3f( 1.0F, 0.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, 1.0F );
  glVertex3f( 0.0F, 1.0F, 1.0F );
  glEnd();
  // Face lateral direita
  glBegin( GL_POLYGON );
  glNormal3f( 1.0F, 0.0F, 0.0F );
  glVertex3f( 1.0F, 0.0F, 1.0F );
  glVertex3f( 1.0F, 0.0F, 0.0F );
  glVertex3f( 1.0F, 1.0F, 0.0F );
  glVertex3f( 1.0F, 1.0F, 1.0F );
  glEnd();
  // Face lateral esquerda
  glBegin( GL_POLYGON );
  glNormal3f( -1.0F, 0.0F, 0.0F );
  glVertex3f( 0.0F, 0.0F, 0.0F );
  glVertex3f( 0.0F, 0.0F, 1.0F );
  glVertex3f( 0.0F, 1.0F, 1.0F );
  glVertex3f( 0.0F, 1.0F, 0.0F );
  glEnd();
  // Face de baixo
  glBegin( GL_POLYGON );
  glNormal3f( 0.0F, -1.0F, 0.0F );
  glVertex3f( 0.0F, 0.0F, 0.0F );
  glVertex3f( 1.0F, 0.0F, 0.0F );
  glVertex3f( 1.0F, 0.0F, 1.0F );
  glVertex3f( 0.0F, 0.0F, 1.0F );
  glEnd();
  // Face de cima
  glBegin( GL_POLYGON );
  glNormal3f( 0.0F, 1.0F, 0.0F );
  glVertex3f( 0.0F, 1.0F, 0.0F );
  glVertex3f( 0.0F, 1.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, 0.0F );
  glEnd();
  // Face traseira
  glBegin( GL_POLYGON );
  glNormal3f( 0.0F, 0.0F, -1.0F );
  glVertex3f( 0.0F, 0.0F, 0.0F );
  glVertex3f( 0.0F, 1.0F, 0.0F );
  glVertex3f( 1.0F, 1.0F, 0.0F );
  glVertex3f( 1.0F, 0.0F, 0.0F );
  glEnd();
}

// --- Funções de Câmera e Transformação ---

/**
 * @brief Constrói e aplica manualmente uma matriz de visão (View Matrix).
 * @details Esta função é uma alternativa ao gluLookAt, calculando a matriz manualmente.
 */
void apply_manual_view_matrix( const qxgl::Vetor3D &eye,
                               const qxgl::Vetor3D &center,
                               const qxgl::Vetor3D &up ) {
  // Calcula a base ortonormal do sistema de coordenadas da câmera.
  const auto Z_AXIS = ( eye - center ).get_unit();  // Vetor "forward" da câmera
  const auto X_AXIS = ( up ^ Z_AXIS ).get_unit();   // Vetor "right" da câmera
  const auto Y_AXIS = ( Z_AXIS ^ X_AXIS );          // Vetor "up" real da câmera

  // Monta a matriz de visão (View Matrix) em ordem de coluna principal (column-major)
  const float VIEW_MATRIX[16] = { X_AXIS.x,          Y_AXIS.x,          Z_AXIS.x,          0.0F,
                                  X_AXIS.y,          Y_AXIS.y,          Z_AXIS.y,          0.0F,
                                  X_AXIS.z,          Y_AXIS.z,          Z_AXIS.z,          0.0F,
                                  -( X_AXIS * eye ), -( Y_AXIS * eye ), -( Z_AXIS * eye ), 1.0F };

  // A sua função original multiplicava uma matriz transposta, então mantemos a consistência.
  // Se a matriz acima for a correta, o ideal seria usar glMultMatrixf.
  glMultMatrixf( VIEW_MATRIX );
}

// --- Callbacks do GLUT ---

void render_scene() {
  GUI::displayInit();

  // 1. Configura a Câmera (Matriz de Visão)
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( glutGUI::cam->e.x,
             glutGUI::cam->e.y,
             glutGUI::cam->e.z,
             glutGUI::cam->c.x,
             glutGUI::cam->c.y,
             glutGUI::cam->c.z,
             glutGUI::cam->u.x,
             glutGUI::cam->u.y,
             glutGUI::cam->u.z );

  // 2. Configura a Iluminação e a Cena Base
  GUI::setLight( 0, 3.0F, 4.0F, 2.0F, true, false );
  GUI::drawOrigin( 0.5F );
  GUI::setColor( 0.5F, 0.5F, 0.7F );
  GUI::drawFloor( 5, 5 );

  // 3. Renderiza os Objetos da Cena
  glPushMatrix();
  {
    // Aplica a transformação do objeto global
    // Nota: A linha original 'GUI::glShearYf( transf., 0.0 )' continha um erro.
    // Assumindo que a intenção era usar um dos componentes do vetor de posição.
    // GUI::glShearYf(g_transform.position.x, 0.0F);

    g_transform.apply_2d();  // Usando o método modernizado

    // Desenha o objeto
    glTranslatef( 1.5F, 1.5F, 0.0F );
    draw_house();
  }
  glPopMatrix();

  // 4. Atualiza o estado com base na entrada do usuário
  g_transform.update_by_mouse_2d();  // Chamando o método com nome modernizado

  GUI::displayEnd();
}

void keyboard_callback( unsigned char key, int mouse_x, int mouse_y ) {
  GUI::keyInit( key, mouse_x, mouse_y );

  switch ( key ) {
    case ' ': glutGUI::trans_obj = !glutGUI::trans_obj; break;
    case 'l': glutGUI::trans_luz = !glutGUI::trans_luz; break;
    case 'a': g_pos_x -= 0.1F; break;
    case 'd': g_pos_x += 0.1F; break;
    case 's': g_pos_y -= 0.1F; break;
    case 'w': g_pos_y += 0.1F; break;
    default : break;
  }
}

// --- Função Principal ---

int main() {
  std::cout << "Aplicacao Iniciada!\n";
  GUI gui( 800, 600, render_scene, keyboard_callback );
  return 0;
}
