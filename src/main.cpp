#include "../lib/qxgl/src/qxgl.hpp"

#include <iostream>

float g_pos_x  = 0.0F;
float g_pos_y  = 0.0F;
float g_radius = 0.0F;

qxgl::Transform g_transform;

void draw_house() {
  glBegin( GL_POLYGON );
  glNormal3f( 0.0F, 0.0F, 1.0F );
  glVertex3f( 0.0F, 0.0F, 1.0F );
  glVertex3f( 1.0F, 0.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, 1.0F );
  glVertex3f( 0.0F, 1.0F, 1.0F );
  glEnd();

  glBegin( GL_POLYGON );
  glNormal3f( 1.0F, 0.0F, 0.0F );
  glVertex3f( 1.0F, 0.0F, 1.0F );
  glVertex3f( 1.0F, 0.0F, 0.0F );
  glVertex3f( 1.0F, 1.0F, 0.0F );
  glVertex3f( 1.0F, 1.0F, 1.0F );
  glEnd();

  glBegin( GL_POLYGON );
  glNormal3f( -1.0F, 0.0F, 0.0F );
  glVertex3f( 0.0F, 0.0F, 0.0F );
  glVertex3f( 0.0F, 0.0F, 1.0F );
  glVertex3f( 0.0F, 1.0F, 1.0F );
  glVertex3f( 0.0F, 1.0F, 0.0F );
  glEnd();

  glBegin( GL_POLYGON );
  glNormal3f( 0.0F, -1.0F, 0.0F );
  glVertex3f( 0.0F, 0.0F, 0.0F );
  glVertex3f( 1.0F, 0.0F, 0.0F );
  glVertex3f( 1.0F, 0.0F, 1.0F );
  glVertex3f( 0.0F, 0.0F, 1.0F );
  glEnd();

  glBegin( GL_POLYGON );
  glNormal3f( 0.0F, 1.0F, 0.0F );
  glVertex3f( 0.0F, 1.0F, 0.0F );
  glVertex3f( 0.0F, 1.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, 1.0F );
  glVertex3f( 1.0F, 1.0F, 0.0F );
  glEnd();

  glBegin( GL_POLYGON );
  glNormal3f( 0.0F, 0.0F, -1.0F );
  glVertex3f( 0.0F, 0.0F, 0.0F );
  glVertex3f( 0.0F, 1.0F, 0.0F );
  glVertex3f( 1.0F, 1.0F, 0.0F );
  glVertex3f( 1.0F, 0.0F, 0.0F );
  glEnd();
}

void apply_manual_view_matrix( const qxgl::Vetor3D &eye,
                               const qxgl::Vetor3D &center,
                               const qxgl::Vetor3D &up ) {
  const auto Z_AXIS = ( eye - center ).get_unit();
  const auto X_AXIS = ( up ^ Z_AXIS ).get_unit();
  const auto Y_AXIS = ( Z_AXIS ^ X_AXIS );

  const float VIEW_MATRIX[16] = { X_AXIS.x,          Y_AXIS.x,          Z_AXIS.x,          0.0F,
                                  X_AXIS.y,          Y_AXIS.y,          Z_AXIS.y,          0.0F,
                                  X_AXIS.z,          Y_AXIS.z,          Z_AXIS.z,          0.0F,
                                  -( X_AXIS * eye ), -( Y_AXIS * eye ), -( Z_AXIS * eye ), 1.0F };

  glMultMatrixf( VIEW_MATRIX );
}

void render_scene() {
  GUI::displayInit();

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( glutGUI::cam->get_eye().x,
             glutGUI::cam->get_eye().y,
             glutGUI::cam->get_eye().z,
             glutGUI::cam->get_center().x,
             glutGUI::cam->get_center().y,
             glutGUI::cam->get_center().z,
             glutGUI::cam->get_up().x,
             glutGUI::cam->get_up().y,
             glutGUI::cam->get_up().z );

  GUI::setLight( 0, 3.0F, 4.0F, 2.0F, true, false );
  GUI::drawOrigin( 0.5F );
  GUI::setColor( 0.5F, 0.5F, 0.7F );
  GUI::drawFloor( 5, 5 );

  glPushMatrix();
  {
    g_transform.apply_2d();
    glTranslatef( 1.5F, 1.5F, 0.0F );
    draw_house();
  }
  glPopMatrix();

  g_transform.update_by_mouse_2d();

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

int main() {
  std::cout << "Aplicacao Iniciada!\n";
  GUI gui( 800, 600, render_scene, keyboard_callback );
  return 0;
}
