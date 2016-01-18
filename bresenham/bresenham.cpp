// Bresenham Line-drawing

#include <Angel.h>

// const int NumPoints = 2;
int NumPoints = 0;   // good to always initialise global variables

//----------------------------------------------------------------------------

void
init( void )
{

  ///////////////////////////////////////////////////////////
  // Viewing volume is calibrated from -1 to 1 in the axes //
  // and is thus a cube of 2 x 2 x 2                       //
  ///////////////////////////////////////////////////////////

  // Specify the endpoint vertices
  //////////////////////////////////////////////////////////////
  // This is where you specify the vertices to draw your      //
  // diagrams - cubes, rectangles, and other regular polygons //
  //////////////////////////////////////////////////////////////
  vec2 vertices[2] = {
    vec2( -1.0, .9 ), vec2( .7, -.9 )
  };

  // Set the global value of NumPoints
  /////////////////////////////////////////////////////////////////
  // You will need to recalculate the number of points to render //
  // for your regular polygon                                    //
  /////////////////////////////////////////////////////////////////
  NumPoints = (vertices[1].x - vertices[0].x) * 10 + 1;
  cout << "NumPoints: " << NumPoints << endl;

  // Holds Del_x and Del_y
  vec2 del = (vertices[1] - vertices[0]);
  // 'decision' parameter
  int p = (int)(2*del.y - del.x);

  // Increment - should ideally be integer increments
  GLfloat inc = (del.x) / (NumPoints - 1);
  GLfloat incy = ((del.y < 0.0) ? (-inc) : 0.0);
  cout << "inc: " << inc << endl;

  // To hold our data
  vec2 points[NumPoints];
  points[0] = vertices[0];
  points[NumPoints - 1] = vertices[1];

  // compute and store N-1 new points
  for ( int i = 1; i < (NumPoints - 1); ++i ) {
    if (p < 0) {
      // Increment in 'x' axis
      points[i] = points[i - 1] + vec2(inc, incy);
      p += 2*del.y;
    }else {
      // Increment in 'x' and 'y' axis
      points[i] = points[i - 1] + vec2(inc, inc);
      p += 2*(del.y - del.x);
    }
  }

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(points),
                points, GL_STATIC_DRAW );

  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vshader.glsl",   // Vertex shader
                               "fshader.glsl" ); // Fragment shader
  glUseProgram( program );

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( loc );
  glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(0) );

  glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
  // delete []points;
}

//----------------------------------------------------------------------------

void
display( void )
{
  glClear( GL_COLOR_BUFFER_BIT );     // clear the window
  glDrawArrays( GL_POINTS, 0, NumPoints );    // draw the points
  glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
  switch ( key ) {
  case 033:
    exit( EXIT_SUCCESS );
    break;
  }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA );
  glutInitWindowSize( 512, 512 );

  // If you are using freeglut, the next two lines will check if
  // the code is truly 3.2. Otherwise, comment them out

  glutInitContextVersion( 3, 2 );
  glutInitContextProfile( GLUT_CORE_PROFILE );

  glutCreateWindow( "Bresenham Line-Drawing Algorithm" );

  glewExperimental = GL_TRUE;
  glewInit();

  glPointSize(10.0);             // Point size in pixels

  init();

  glutDisplayFunc( display );
  glutKeyboardFunc( keyboard );

  glutMainLoop();
  return 0;
}
