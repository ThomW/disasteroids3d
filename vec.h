
#include <math.h>

typedef float vec_t;

struct vec3_t
{
   vec3_t() {v[0] = v[1] = v[2] = 0;}
   vec3_t(vec_t px, vec_t py, vec_t pz) {v[0] = px; v[1] = py; v[2] = pz;}
   vec3_t(vec3_t &pVec) {v[0] = pVec.v[0]; v[1] = pVec.v[1]; v[2] = pVec.v[2];}
   vec3_t(vec_t *pVec) {v[0] = pVec[0]; v[1] = pVec[1]; v[2] = pVec[2];}

   vec3_t operator=(vec3_t &pVec)
    {return vec3_t(v[0] = pVec.v[0], v[1] = pVec.v[1], v[2] = pVec.v[2]);}
   vec3_t operator=(vec_t *ptr)
    {return vec3_t(v[0] = ptr[0], v[1] = ptr[1], v[2] = ptr[2]);}
   int operator==(vec3_t &pVec)
    {return (v[0] == pVec.v[0] && v[1] == pVec.v[1] && v[2] == pVec.v[2]);}
   int operator==(float *pVec)
    {return (v[0] == pVec[0] && v[1] == pVec[1] && v[2] == pVec[2]);}
   inline int operator!=(vec3_t &pVec)
    {return !(pVec == (*this));}
   inline int operator!=(float *pVec)
    {return !(pVec == (*this));}

   vec3_t operator+=(vec3_t &pVec);  
   vec3_t operator-=(vec3_t &pVec);
   vec3_t operator*=(vec3_t &pVec);
   vec3_t operator*=(vec_t val);  
   vec3_t operator/=(vec3_t &pVec);
   vec3_t operator/=(vec_t val);

   vec3_t operator+(vec3_t &pVec)
    {return vec3_t(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);}
   vec3_t operator-(vec3_t &pVec)
    {return vec3_t(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);}
   vec3_t operator*(vec3_t &pVec)
    {return vec3_t(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);}
   vec3_t operator*(vec_t val)
    {return vec3_t(v[0] * val, v[1] * val, v[2] * val);}
   vec3_t operator/(vec3_t &pVec)  
    {return vec3_t(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2]);}
   vec3_t operator/(vec_t val)
    {return vec3_t(v[0] / val, v[1] / val, v[2] / val);}

   void Clear(void) {v[0] = v[1] = v[2] = 0;}
   void Normalize(void);
   vec_t Length(void);
   vec_t Dot(vec3_t &pVec) {return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];}
   void Cross(vec3_t &p, vec3_t &q);

   void Set(float x, float y, float z) {v[0] = x; v[1] = y; v[2] = z;}
   
   vec_t X(void) {return v[0];}
   vec_t Y(void) {return v[1];}
   vec_t Z(void) {return v[2];}
   void X(vec_t nx) {v[0] = nx;}
   void Y(vec_t ny) {v[1] = ny;}
   void Z(vec_t nz) {v[2] = nz;}

   const vec_t &operator[](int ndx) const {return v[ndx];}
   vec_t &operator[](int ndx) {return v[ndx];}
   operator vec_t*(void) {return v;}

   void Clamp(int min, int max);

   void RotateX(float amnt);
   void RotateY(float amnt);
   void RotateZ(float amnt);

/*
   friend ostream &operator<<(ostream &os, vec3_t &vec)
    {os << "("<< vec.v[0] << " " << vec.v[1] << " " << vec.v[2] << ")"; return os;}
 */
	
protected:
   vec_t v[3];
};

inline vec3_t vec3_t::operator+=(vec3_t &pVec)
{
   vec3_t ret;

   ret = *this = *this + pVec;

   return ret;
}

inline vec3_t vec3_t::operator-=(vec3_t &pVec)
{
   vec3_t ret;

   ret = *this = *this - pVec;

   return ret;
}

inline vec3_t vec3_t::operator*=(vec3_t &pVec)
{
   vec3_t ret;

   ret = *this = *this * pVec;

   return ret;
}

inline vec3_t vec3_t::operator*=(vec_t val)
{
   vec3_t ret;

   ret = *this = *this * val;

   return ret;
}

inline vec3_t vec3_t::operator/=(vec3_t &pVec)
{
   vec3_t ret;

   ret = *this = *this / pVec;

   return ret;
}

inline vec3_t vec3_t::operator/=(vec_t val)
{
   vec3_t ret;

   ret = *this = *this / val;

   return ret;
}

inline void vec3_t::Normalize(void) 
{
	vec_t length, len = 0;

	length = Length();

	if (length == 0)
		return;

	len = 1.0f / length;

	v[0] *= len;
	v[1] *= len;
	v[2] *= len;
}

inline float vec3_t::Length(void) 
{
   double length = 0;
	
   length = (v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]);
	
	return (vec_t) sqrt(length);
}

inline void vec3_t::Cross(vec3_t &p, vec3_t &q)
{
	v[0] = p.v[1] * q.v[2] - p.v[2] * q.v[1];
	v[1] = p.v[2] * q.v[0] - p.v[0] * q.v[2];
	v[2] = p.v[0] * q.v[1] - p.v[1] * q.v[0];
}

inline void vec3_t::Clamp(int min, int max)
{
   if (v[0] > max || v[0] < min)
      v[0] = 0;

   if (v[1] > max || v[1] < min)
      v[1] = 0;

   if (v[2] > max || v[2] < min)
      v[2] = 0;   
}

inline void vec3_t::RotateX(float amnt)
{
  float s = (float)sin(amnt);
  float c = (float)cos(amnt);
  float y = v[1];
  float z = v[2];
  
  v[1] = (y * c) - (z * s);
  v[2] = (y * s) + (z * c);
}

inline void vec3_t::RotateY(float amnt)
{
  float s = (float)sin(amnt);
  float c = (float)cos(amnt);
  float x = v[0];
  float z = v[2];
  
  v[0] = (x * c) + (z * s);
  v[2] = (z * c) - (x * s);
}

inline void vec3_t::RotateZ(float amnt)
{
  float s = (float)sin(amnt);
  float c = (float)cos(amnt);
  float x = v[0];
  float y = v[1];
  
  v[0] = (x * c) - (y * s);
  v[1] = (y * c) + (x * s);
}
