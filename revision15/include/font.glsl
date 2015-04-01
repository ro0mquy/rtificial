#define A_ vec2(0.,0.)
#define B_ vec2(1.,0.)
#define C_ vec2(2.,0.)

//#define D_ vec2(0.,1.)
#define E_ vec2(1.,1.)
//#define F_ vec2(2.,1.)

#define G_ vec2(0.,2.)
#define H_ vec2(1.,2.)
#define I_ vec2(2.,2.)

#define J_ vec2(0.,3.)
#define K_ vec2(1.,3.)
#define L_ vec2(2.,3.)

#define M_ vec2(0.,4.)
#define N_ vec2(1.,4.)
#define O_ vec2(2.,4.)

//#define P_ vec2(0.,5.)
//#define Q_ vec2(1.,5.)
//#define R_ vec2(1.,5.)

#define S_ vec2(0.,6.)
#define T_ vec2(1.,6.)
#define U_ vec2(2.0,6.)

#define A(p) min(min(min(t(G_,I_,p), t(I_,O_,p)), min(t(O_,M_, p), t(M_,J_,p))), t(J_,L_,p))
#define B(p) min(min(t(A_,M_,p), t(M_,O_,p)), min(t(O_,I_, p), t(I_,G_,p)))
#define C(p) min(min(t(I_,G_,p), t(G_,M_,p)), t(M_,O_,p))
#define D(p) min(min(t(C_,O_,p), t(O_,M_,p)), min(t(M_,G_,p), t(G_,I_,p)))
#define E(p) min(min(min(t(O_,M_,p), t(M_,G_,p)), min(t(G_,I_,p), t(I_,L_,p))), t(L_,J_,p))
#define F(p) min(min(t(C_,B_,p), t(B_,N_,p)), t(G_,I_,p))
#define G(p) min(min(min(t(O_,M_,p), t(M_,G_,p)), min(t(G_,I_,p), t(I_,U_,p))), t(U_,S_,p))
#define H(p) min(min(t(A_,M_,p), t(G_,I_,p)), t(I_,O_,p))
#define I(p) min(t(E_,E_,p), t(H_,N_,p))
#define J(p) min(min(t(E_,E_,p), t(H_,T_,p)), t(T_,S_,p))
#define K(p) min(min(t(A_,M_,p), t(M_,I_,p)), t(K_,O_,p))
#define L(p) t(B_,N_,p)
#define M(p) min(min(t(M_,G_,p), t(G_,I_,p)), min(t(H_,N_,p), t(I_,O_,p)))
#define N(p) min(min(t(M_,G_,p), t(G_,I_,p)), t(I_,O_,p))
#define O(p) min(min(t(G_,I_,p), t(I_,O_,p)), min(t(O_,M_, p), t(M_,G_,p)))
#define P(p) min(min(t(S_,G_,p), t(G_,I_,p)), min(t(I_,O_,p), t(O_,M_, p)))
#define Q(p) min(min(t(U_,I_,p), t(I_,G_,p)), min(t(G_,M_,p), t(M_,O_, p)))
#define R(p) min(t(M_,G_,p), t(G_,I_,p))
#define S(p) min(min(min(t(I_,G_,p), t(G_,J_,p)), min(t(J_,L_,p), t(L_,O_,p))), t(O_,M_,p))
#define T(p) min(min(t(B_,N_,p), t(N_,O_,p)), t(G_,I_,p))
#define U(p) min(min(t(G_,M_,p), t(M_,O_,p)), t(O_,I_,p))
#define V(p) min(min(t(G_,J_,p), t(J_,N_,p)), min(t(N_,L_,p), t(L_,I_,p)))
#define W(p) min(min(t(G_,M_,p), t(M_,O_,p)), min(t(N_,H_,p), t(O_,I_,p)))
#define X(p) min(t(G_,O_,p), t(I_,M_,p))
#define Y(p) min(min(t(G_,M_,p), t(M_,O_,p)), min(t(I_,U_,p), t(U_,S_,p)))
#define Z(p) min(min(t(G_,I_,p), t(I_,M_,p)), t(M_,O_,p))
#define STOP(p) t(N_,N_,p)

//-----------------------------------------------------------------------------------
float t(vec2 v, vec2 w, vec2 p)
{
	// Return minimum distance between line segment vw and point p
	float l2 = (v.x - w.x)*(v.x - w.x) + (v.y - w.y)*(v.y - w.y); //length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0) {
		return distance(p, v);   // v == w case
	}

	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of point p onto the line.  It falls where t = [(p-v) . (w-v)] / |w-v|^2
	float t = dot(p - v, w - v) / l2;
	if(t < 0.0) {
		// Beyond the 'v' end of the segment
		return distance(p, v);
	} else if (t > 1.0) {
		return distance(p, w);  // Beyond the 'w' end of the segment
	}
	vec2 projection = v + t * (w - v);  // Projection falls on the segment
	return distance(p, projection);
}
