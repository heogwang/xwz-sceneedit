/* Arcball, written by Bradley Smith, March 24, 2006
* arcball.cpp is free to use and modify for any purpose, with no
* restrictions of copyright or license.
*
* See arcball.h for usage details.
*/

#include <windows.h>
#include "arcball.h"
#include <gl/glu.h>
#include <gl/gl.h>
#include <iostream>
using namespace std;

arcball::arcball( void )
{
	ab_quat=new GLfloat[16];
	quatidentity(ab_quat);

	ab_last=new GLfloat[16];
	quatidentity(ab_last);

	ab_next=new GLfloat[16];
	quatidentity(ab_next);

	ab_glp=new GLdouble[16];
	quatidentity(ab_glp);

	ab_glm=new GLdouble[16];
	quatidentity(ab_glm);

	ab_glv=new int[4];
	ab_glv[0]=ab_glv[1]=0;
	ab_glv[2]=640;
	ab_glv[3]=480;

	ab_zoom=ab_zoom2=ab_sphere=ab_sphere2=ab_edge=1.0;

	ab_start=ab_curr=ab_eye=ab_eyedir=vec(0,0,1);
	ab_up=vec(0,1,0);
	center=vec(0,0,0);
}


arcball::~arcball( void )
{
	delete []ab_quat;
	delete []ab_last;
	delete []ab_next;
	delete []ab_glp;
	delete []ab_glm;
	delete []ab_glv;
}

void arcball::arcball_setzoom(float radius, vec eye, vec up)
{
	ab_eye = eye; // store eye vector
	ab_zoom2 = len2(ab_eye);
	ab_zoom = sqrt(ab_zoom2); // store eye distance
	ab_sphere = radius; // sphere radius
	ab_sphere2 = ab_sphere * ab_sphere;
	ab_eyedir =ab_eye * ((float)1.0 / ab_zoom); // eye方向的单位向量
	ab_edge = ab_sphere2 / ab_zoom; // plane of visible edge

	glGetDoublev(GL_PROJECTION_MATRIX,ab_glp);
	glGetIntegerv(GL_VIEWPORT,ab_glv);
}

// affect the arcball's orientation on openGL
void arcball::arcball_multiMatrix() 
{ 
	glMultMatrixf(ab_quat); 
}


// reset the rotation matrix
void arcball::quatidentity(GLfloat* q)
{ 
	q[0]=1;  q[1]=0;  q[2]=0;  q[3]=0;
	q[4]=0;  q[5]=1;  q[6]=0;  q[7]=0;
	q[8]=0;  q[9]=0;  q[10]=1; q[11]=0;
	q[12]=0; q[13]=0; q[14]=0; q[15]=1; 
}



void arcball::quatidentity( GLdouble* q )
{
	q[0]=1;  q[1]=0;  q[2]=0;  q[3]=0;
	q[4]=0;  q[5]=1;  q[6]=0;  q[7]=0;
	q[8]=0;  q[9]=0;  q[10]=1; q[11]=0;
	q[12]=0; q[13]=0; q[14]=0; q[15]=1;
}

// convert the quaternion into a rotation matrix
//根据四元组生成旋转矩阵
void arcball::quaternion(GLfloat* q, GLfloat x, GLfloat y, GLfloat z, GLfloat w,vec center)
{
	GLfloat x2 = x*x;
	GLfloat y2 = y*y;
	GLfloat z2 = z*z;
	GLfloat xy = x*y;
	GLfloat xz = x*z;
	GLfloat yz = y*z;
	GLfloat wx = w*x;
	GLfloat wy = w*y;
	GLfloat wz = w*z;

	q[0] = 1 - 2*y2 - 2*z2;
	q[1] = 2*xy + 2*wz;
	q[2] = 2*xz - 2*wy;

	q[4] = 2*xy - 2*wz;
	q[5] = 1 - 2*x2 - 2*z2;
	q[6] = 2*yz + 2*wx;

	q[8] = 2*xz + 2*wy;
	q[9] = 2*yz - 2*wx;
	q[10]= 1 - 2*x2 - 2*y2;

	q[12]=-(q[0]*center[0]+q[4]*center[1]+q[8]*center[2]);
	q[13]=-(q[1]*center[0]+q[5]*center[1]+q[9]*center[2]);
	q[14]=-(q[2]*center[0]+q[6]*center[1]+q[10]*center[2]);

}

// 生成平移矩阵
void arcball::quattrans(GLfloat* dest, GLfloat x, GLfloat y, GLfloat z)
{
	dest[12]+=x;
	dest[13]+=y;
	dest[14]+=z;
}

void arcball::quatscale(GLfloat* q, GLfloat x,GLfloat y,GLfloat z)
{
	quatidentity(q);
	q[0]=x;
	q[5]=y;
	q[10]=z;
}

// copy a rotation matrix
void arcball::quatcopy(GLfloat* dst, GLfloat* src)
{ dst[0]=src[0]; dst[1]=src[1]; dst[2]=src[2];dst[3]=src[3];
dst[4]=src[4]; dst[5]=src[5]; dst[6]=src[6];dst[7]=src[7];
dst[8]=src[8]; dst[9]=src[9]; dst[10]=src[10]; dst[11]=src[11];
dst[12]=src[12];dst[13]=src[13];dst[14]=src[14];dst[15]=src[15];
}

// multiply two rotation matrices
void arcball::quatnext(GLfloat* dest, GLfloat* left, GLfloat* right)
{
	dest[0] = left[0]*right[0] + left[1]*right[4] + left[2] *right[8]+left[3]*right[12];
	dest[1] = left[0]*right[1] + left[1]*right[5] + left[2] *right[9]+left[3]*right[13];
	dest[2] = left[0]*right[2] + left[1]*right[6] + left[2] *right[10]+left[3]*right[14];
	//dest[3]= left[0]*right[3] + left[1]*right[7] + left[2] *right[11]+left[3]*right[15];

	dest[4] = left[4]*right[0] + left[5]*right[4] + left[6] *right[8]+left[7]*right[12];
	dest[5] = left[4]*right[1] + left[5]*right[5] + left[6] *right[9]+left[7]*right[13];
	dest[6] = left[4]*right[2] + left[5]*right[6] + left[6] *right[10]+left[7]*right[14];
	//dest[7]= left[4]*right[3] + left[5]*right[7] + left[6] *right[11]+left[7]*right[15];


	dest[8] = left[8]*right[0] + left[9]*right[4] + left[10]*right[8]+left[11]*right[12];
	dest[9] = left[8]*right[1] + left[9]*right[5] + left[10]*right[9]+left[11]*right[13];
	dest[10]= left[8]*right[2] + left[9]*right[6] + left[10]*right[10]+left[11]*right[14];
	//dest[11]= left[8]*right[3] + left[9]*right[7] + left[10]*right[11]+left[11]*right[15];

	//dest[12]=left[12]*right[0]+left[13]*right[4]+left[14]*right[8]+left[15]*right[12];
	//dest[13]=left[12]*right[1]+left[13]*right[5]+left[14]*right[9]+left[15]*right[13];
	//dest[14]=left[12]*right[2]+left[13]*right[6]+left[14]*right[10]+left[15]*right[14];
	//dest[15]=left[12]*right[3]+left[13]*right[7]+left[14]*right[11]+left[15]*right[15];

	//dest[15]=1;

	//dest[12]=left[12]*right[0]+left[13]*right[4]+left[14]*right[8]+left[15]*right[12];
	//dest[13]=left[12]*right[1]+left[13]*right[5]+left[14]*right[9]+left[15]*right[13];
	//dest[14]=left[12]*right[2]+left[13]*right[6]+left[14]*right[10]+left[15]*right[14];
	//dest[15]=left[12]*right[3]+left[13]*right[7]+left[14]*right[11]+left[15]*right[15];

	//cout<<left[0]<<"\t"<<left[1]<<"\t"<<left[2]<<"\t"<<left[3]<<endl;
	//cout<<left[4]<<"\t"<<left[5]<<"\t"<<left[6]<<"\t"<<left[7]<<endl;
	//cout<<left[8]<<"\t"<<left[9]<<"\t"<<left[10]<<"\t"<<left[11]<<endl;
	//cout<<left[12]<<"\t"<<left[13]<<"\t"<<left[14]<<"\t"<<left[15]<<endl;
	//cout<<endl;
	//cout<<endl;

	//cout<<right[0]<<"\t"<<right[1]<<"\t"<<right[2]<<"\t"<<right[3]<<endl;
	//cout<<right[4]<<"\t"<<right[5]<<"\t"<<right[6]<<"\t"<<right[7]<<endl;
	//cout<<right[8]<<"\t"<<right[9]<<"\t"<<right[10]<<"\t"<<right[11]<<endl;
	//cout<<right[12]<<"\t"<<right[13]<<"\t"<<right[14]<<"\t"<<right[15]<<"\t"<<endl;
	//cout<<endl;
	//cout<<endl;

	//cout<<dest[0]<<"\t"<<dest[1]<<"\t"<<dest[2]<<"\t"<<dest[3]<<endl;
	//cout<<dest[4]<<"\t"<<dest[5]<<"\t"<<dest[6]<<"\t"<<dest[7]<<endl;
	//cout<<dest[8]<<"\t"<<dest[9]<<"\t"<<dest[10]<<"\t"<<dest[11]<<endl;
	//cout<<dest[12]<<"\t"<<dest[13]<<"\t"<<dest[14]<<"\t"<<dest[15]<<endl;
	//cout<<endl;
	//cout<<endl;

}

// find the intersection with the plane through the visible edge
vec arcball::edge_coords(vec m)
{
	// find the intersection of the edge plane and the ray
	float t = (ab_edge - ab_zoom) / (ab_eyedir ^ m);
	vec a = ab_eye + (m*t);
	// find the direction of the eye-axis from that point
	// along the edge plane
	vec c = (ab_eyedir * ab_edge) - a;

	// find the intersection of the sphere with the ray going from
	// the plane outside the sphere toward the eye-axis.
	float ac = (a^c);
	float c2 = (c^c);
	float q = ( 0.0 - ac - sqrt( (ac*ac) - c2*(len2(a)-ab_sphere2) ) ) / c2;
	vec result=a+(c*q);
	normalize(result);
	return result;
}

// find the intersection with the sphere
vec arcball::sphere_coords(GLdouble mx, GLdouble my)
{
	GLdouble ax,ay,az;

	bool temp = gluUnProject(mx,my,0,ab_glm,ab_glp,ab_glv,&ax,&ay,&az);
	vec m = vec((float)ax,(float)ay,(float)az) - ab_eye;

	// mouse position represents ray: eye + t*m
	// intersecting with a sphere centered at the origin
	GLfloat a = m^m;
	GLfloat b = (ab_eye^m);
	GLfloat root = (b*b) - a*(ab_zoom2 - ab_sphere2);
	if(root <= 0) return edge_coords(m);  //这里root是指的代尔塔，如果其小于0，则只移动背景，因为说明跟球没有交点，肯定就不移动球了
	GLfloat t = (0.0 - b - sqrt(root)) / a;
	vec result=ab_eye+(m*t); 
	normalize(result);
	return result;
}

void arcball::arcball_reset()
{
	quatidentity(ab_quat);
	quatidentity(ab_last);
}

// begin arcball rotation
void arcball::arcball_rotStart(int mx, int my)
{
	// saves a copy of the current rotation for comparison
	quatcopy(ab_last,ab_quat);
	ab_start = sphere_coords((GLdouble)mx,(GLdouble)my);  // 找到球上坐标
}

// update current arcball rotation
void arcball::arcball_rotMove(int mx, int my)
{
	ab_curr = sphere_coords((GLdouble)mx,(GLdouble)my);
	if(ab_curr==(ab_start))
	{ // avoid potential rare divide by tiny（微小的）
		quatcopy(ab_quat,ab_last);
		return;
	}
	// use a dot product to get the angle between them
	// use a cross product to get the vector to rotate around
	GLfloat cos2a = ab_start^ab_curr;
	GLfloat sina = sqrt((1.0 - cos2a)*0.5);
	GLfloat cosa = sqrt((1.0 + cos2a)*0.5);
	vec cross = ab_start%ab_curr;
	normalize(cross);
	cross*=sina;
	quaternion(ab_next,cross[0],cross[1],cross[2],cosa,center);

	//GLfloat temp[16];
	//quatidentity(temp);
	//temp[12]=-center[0];
	//temp[13]=-center[1];
	//temp[14]=-center[2];
	//GLfloat dest[16];
	//quatnext(dest,ab_next,temp);
	// update the rotation matrix
	//vec temp(ab_last[12],ab_last[13],ab_last[14]);

	quatnext(ab_quat,ab_last,ab_next);
	quattrans(ab_quat,center[0],center[1],center[2]);
}

void arcball::arcball_trans(GLfloat dx, GLfloat dy, GLfloat dz)
{
	quattrans(ab_quat,dx,dy,dz);
	//// 如果是鼠标事件，在点击的时候记录，这里就不需要添加了
	quatcopy(ab_last,ab_quat);
}

void arcball::arcball_scale(GLfloat sx,GLfloat sy,GLfloat sz)
{
	quatscale(ab_next,sx,sy,sz);
	quatnext(ab_quat,ab_last,ab_next);
	quatcopy(ab_last,ab_quat);
}

void arcball::arcball_tranStart( int mx, int my )
{
	quatcopy(ab_last,ab_quat);
	GLdouble ax,ay,az;
	gluUnProject(mx,my,0,ab_glm,ab_glp,ab_glv,&ax,&ay,&az); // 找到坐标系中点
	ab_start=vec((float)ax,(float)ay,(float)az);
}

void arcball::arcball_tranMove( int mx, int my )
{
	GLdouble ax,ay,az;
	gluUnProject(mx,my,0,ab_glm,ab_glp,ab_glv,&ax,&ay,&az); // 找到坐标系中点
	ab_curr=vec((float)ax,(float)ay,(float)az);
	if(ab_curr==(ab_start))
	{ // avoid potential rare divide by tiny（微小的）
		quatcopy(ab_quat,ab_last);
		return;
	}

	vec step=ab_curr-ab_start;
	arcball_trans(step[0],step[1],step[2]);
	ab_start=ab_curr;
}

void arcball::arcball_setCenter( vec center )
{
	this->center=center;
}
