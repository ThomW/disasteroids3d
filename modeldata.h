
#include "game.h"

/*
// V3 Player Data -- Generated by 3D Explorer -- WITH TEXTURES
//
// 9 Verticies
// 18 Texture Coordinates
// 18 Normals
// 12 Triangles
static BYTE player_face_indicies[12][9] = {
	{1,5,3 ,0,1,0 ,0,1,2 }, {1,3,0 ,0,0,2 ,0,2,3 }, {4,1,0 ,3,4,3 ,4,5,6 },
	{4,0,2 ,3,3,5 ,4,6,7 }, {2,0,6 ,6,7,7 ,3,8,9 }, {0,3,6 ,7,8,7 ,8,6,9 },
	{5,1,7 ,9,10,10 ,6,8,9 }, {1,4,7 ,10,11,10 ,8,3,9 }, {7,6,3 ,12,13,12 ,10,11,12 },
	{7,3,5 ,12,12,14 ,10,12,13 }, {4,2,7 ,15,16,16 ,14,15,16 }, {7,2,6 ,16,16,17 ,16,15,17 }
};
static GLfloat player_verticies [9][3] = {
{0.5f,6.32393e-08f,-0.166667f},{0.5f,6.15921e-08f,0.166667f},{-0.5f,0.333333f,-0.166667f},
{-0.5f,-0.333333f,-0.166667f},{-0.5f,0.333333f,0.166666f},{-0.5f,-0.333333f,0.166667f},
{-0.333333f,2.68131e-08f,-0.166667f},{-0.333333f,2.5166e-08f,0.166667f},{-0.5f,0.333333f,0.166667f}
};
static GLfloat player_normals [18][3] = {
{0.316228f,-0.948683f,-2.34396e-09f},{0.316228f,-0.948683f,0.0f},{0.316228f,-0.948683f,-4.68792e-09f},
{0.316228f,0.948683f,2.34396e-09f},{0.316228f,0.948683f,4.68792e-09f},{0.316228f,0.948683f,0.0f},
{-4.76837e-08f,5.96046e-09f,-1.0f},{-4.76837e-08f,2.22045e-16f,-1.0f},{-4.76837e-08f,-5.96046e-09f,-1.0f},
{4.76837e-08f,-2.38419e-08f,1.0f},{4.76837e-08f,9.83477e-07f,1.0f},{4.76836e-08f,1.9908e-06f,1.0f},
{-0.894427f,0.447214f,1.10495e-09f},{-0.894427f,0.447214f,2.2099e-09f},{-0.894427f,0.447214f,0.0f},
{-0.894427f,-0.447214f,0.0f},{-0.894427f,-0.447214f,-1.10495e-09f},{-0.894427f,-0.447214f,-2.20991e-09f}
};
static GLfloat player_textures [18][2] = {
{0.0f,0.0f},{0.466391f,0.0f},{0.466391f,1.0f},
{0.0f,1.0f},{0.631285f,0.0f},{1.0f,0.0f},
{1.0f,1.0f},{0.631285f,1.0f},{0.5f,0.0f},
{0.5f,0.833333f},{0.0f,0.333333f},{0.0f,0.416667f},
{0.5f,0.416667f},{0.5f,0.333333f},{0.75f,0.333333f},
{0.75f,0.416667f},{1.0f,0.333333f},{1.0f,0.416667f}
};
*/


// New player ship
//
// 24 Verticies
// 24 Texture Coordinates
// 32 Normals
// 38 Triangles
static BYTE player_face_indicies[38][9] = {
// Object: 7
	{0,1,2 ,0,1,2 ,0,1,2 }, {0,2,3 ,0,2,2 ,0,2,3 }, {4,5,6 ,3,4,5 ,4,5,6 },
	{4,6,7 ,3,5,6 ,4,6,7 }, {7,6,8 ,6,5,6 ,7,6,8 }, {1,7,8 ,6,6,6 ,1,7,8 },
	{1,8,2 ,6,6,6 ,1,8,2 }, {8,9,2 ,6,6,6 ,8,9,2 }, {4,10,5 ,3,7,4 ,4,10,5 },
	{5,10,11 ,4,7,6 ,5,10,11 }, {5,11,12 ,4,6,6 ,5,11,12 }, {11,13,12 ,6,6,6 ,11,13,12 },
	{12,13,9 ,6,6,6 ,12,13,9 }, {8,12,9 ,6,6,6 ,8,12,9 }, {14,7,1 ,8,9,1 ,14,7,1 },
	{14,1,0 ,8,1,0 ,14,1,0 }, {15,4,14 ,10,11,8 ,15,4,14 }, {14,4,7 ,8,11,9 ,14,4,7 },
	{15,16,10 ,12,12,7 ,15,16,10 }, {15,10,4 ,12,7,3 ,15,10,4 },
	{10,16,17 ,13,14,15 ,10,16,17 }, {10,17,11 ,13,15,16 ,10,17,11 },
	{11,17,13 ,16,15,17 ,11,17,13 }, {13,17,18 ,17,15,18 ,13,17,18 },
	{13,18,9 ,17,18,19 ,13,18,9 }, {9,18,19 ,19,18,19 ,9,18,19 },
	{3,2,9 ,20,20,20 ,3,2,9 }, {3,9,19 ,20,20,20 ,3,9,19 }, {8,20,21 ,21,22,23 ,8,20,21 },
	{8,21,12 ,21,23,21 ,8,21,12 }, {22,23,21 ,24,25,23 ,22,23,21 },
	{22,21,20 ,24,23,22 ,22,21,20 }, {21,23,5 ,26,27,26 ,21,23,5 },
	{21,5,12 ,26,26,28 ,21,5,12 }, {6,5,23 ,5,4,25 ,6,5,23 }, {6,23,22 ,5,25,24 ,6,23,22 },
	{22,20,8 ,29,30,29 ,22,20,8 }, {22,8,6 ,29,29,31 ,22,8,6 }
};
static GLfloat player_verticies [24][3] = {
{-0.277778f,-0.5f,-0.1f},{-0.273364f,-0.374637f,0.00037f},{-0.440189f,-0.374637f,0.00037f},
{-0.5f,-0.5f,-0.1f},{0.310524f,-0.040986f,0.00037f},{0.083387f,0.047649f,0.00037f},
{0.083387f,-0.047649f,0.00037f},{-0.106539f,-0.207811f,0.00037f},{-0.421861f,-0.143432f,0.00037f},
{-0.440189f,0.376077f,0.00037f},{0.310524f,0.059776f,0.00037f},{-0.106539f,0.209252f,0.00037f},
{-0.421861f,0.143432f,0.00037f},{-0.273364f,0.376077f,0.00037f},{-0.055556f,-0.277778f,-0.1f},
{0.5f,-0.055556f,-0.1f},{0.5f,0.078667f,-0.1f},{-0.055556f,0.277778f,-0.1f},
{-0.277778f,0.5f,-0.1f},{-0.5f,0.5f,-0.1f},{-0.363453f,-0.086295f,0.064088f},
{-0.363453f,0.080771f,0.064088f},{-0.210162f,-0.066303f,0.1f},{-0.210162f,0.052492f,0.1f}
};
static GLfloat player_normals [32][3] = {
{0.186028f,-0.618071f,0.763795f},{0.372055f,-0.588076f,0.718152f},{0.0f,-0.624997f,0.780627f},
{0.119665f,0.0f,0.992814f},{0.0648311f,0.0f,0.997896f},{0.130226f,0.0f,0.991484f},
{0.0f,0.0f,1.0f},{0.24117f,0.0f,0.970483f},{0.416537f,-0.632822f,0.652713f},
{0.416537f,-0.632821f,0.652714f},{0.284952f,-0.712382f,0.641338f},{0.284953f,-0.712382f,0.641337f},
{0.468103f,0.0f,0.883674f},{0.261647f,0.730042f,0.631331f},{0.261646f,0.730041f,0.631333f},
{0.407122f,0.644727f,0.646976f},{0.407122f,0.644728f,0.646976f},{0.374076f,0.591707f,0.714108f},
{0.187038f,0.6223f,0.760105f},{0.0f,0.629393f,0.777087f},{-0.859041f,0.0f,0.511907f},
{-0.737155f,0.0f,0.675724f},{-0.505091f,0.0f,0.863066f},{-0.505091f,0.0f,0.863066f},
{-0.0465262f,0.0f,0.998917f},{0.143126f,0.0f,0.989704f},{0.130675f,0.878352f,0.459806f},
{0.110529f,0.953822f,0.279296f},{0.145766f,0.768902f,0.622529f},{0.0531193f,-0.841098f,0.538269f},
{-0.0719146f,-0.708915f,0.701618f},{0.174977f,-0.922992f,0.342737f}
};
static GLfloat player_textures [24][2] = {
{0.244729f,0.045186f},{0.248967f,0.159263f},{0.088813f,0.159263f},
{0.031393f,0.045186f},{0.809509f,0.462878f},{0.591454f,0.543533f},
{0.591454f,0.456815f},{0.409121f,0.311071f},{0.106408f,0.369654f},
{0.088813f,0.842394f},{0.809509f,0.554569f},{0.409121f,0.690588f},
{0.106408f,0.630694f},{0.248967f,0.842394f},{0.458066f,0.247402f},
{0.991409f,0.449619f},{0.991409f,0.571758f},{0.458066f,0.752945f},
{0.244729f,0.955162f},{0.031393f,0.955162f},{0.162479f,0.421648f},
{0.162479f,0.5787f},{0.309641f,0.439839f},{0.309641f,0.560508f}
};

// Rocks created in TrueSpace, data points generated in 3D Explorer
//
// 32 Verticies
// 69 Texture Coordinates
// 68 Normals
// 60 Triangles
static BYTE rock_face_indicies[60][9] = {
	{2,1,0 ,0,1,2 ,0,1,2 }, {1,3,0 ,1,3,2 ,1,3,2 }, {3,4,0 ,3,4,2 ,3,4,2 },
	{4,5,0 ,4,5,2 ,4,5,2 }, {5,6,0 ,5,6,2 ,5,6,2 }, {6,2,0 ,6,0,2 ,6,0,2 },
	{1,2,7 ,1,0,7 ,1,0,7 }, {1,7,8 ,1,7,8 ,8,9,10 }, {1,8,3 ,9,10,11 ,11,12,13 },
	{3,8,9 ,11,10,12 ,13,12,14 }, {4,3,10 ,4,13,14 ,4,3,15 }, {10,3,9 ,14,13,15 ,16,17,18 },
	{4,10,5 ,4,16,5 ,4,15,5 }, {5,10,11 ,5,16,17 ,5,15,19 }, {12,6,5 ,18,19,5 ,20,21,22 },
	{12,5,11 ,18,5,20 ,23,5,19 }, {7,2,6 ,21,0,22 ,7,0,6 }, {7,6,12 ,21,22,23 ,9,24,25 },
	{8,7,13 ,8,7,24 ,10,9,26 }, {8,13,14 ,8,24,25 ,10,26,27 }, {8,14,9 ,10,26,12 ,12,28,14 },
	{9,14,15 ,12,26,27 ,14,28,29 }, {10,9,15 ,14,12,27 ,15,30,31 },
	{10,15,16 ,14,28,29 ,16,32,33 }, {11,10,16 ,17,16,30 ,34,16,33 },
	{11,16,17 ,17,30,31 ,34,33,35 }, {18,12,11 ,32,18,33 ,36,20,37 },
	{18,11,17 ,32,33,34 ,36,37,38 }, {7,12,18 ,21,23,35 ,9,25,39 },
	{7,18,13 ,21,35,36 ,9,39,26 }, {14,13,19 ,25,37,38 ,27,26,40 },
	{14,19,20 ,25,38,39 ,27,40,41 }, {14,20,21 ,26,40,41 ,28,42,43 },
	{14,21,15 ,26,41,27 ,28,43,29 }, {16,15,22 ,29,28,42 ,33,32,44 },
	{22,15,21 ,42,28,43 ,44,32,45 }, {17,16,22 ,31,30,44 ,35,33,44 },
	{17,22,23 ,31,44,45 ,35,44,46 }, {24,18,23 ,46,32,47 ,47,36,48 },
	{23,18,17 ,47,32,34 ,48,36,38 }, {13,18,19 ,48,35,49 ,26,39,40 },
	{19,18,24 ,49,35,50 ,40,39,49 }, {20,19,25 ,39,38,51 ,41,40,50 },
	{25,19,26 ,51,38,52 ,50,40,51 }, {20,25,27 ,53,54,55 ,52,53,54 },
	{20,27,21 ,53,55,56 ,42,55,43 }, {22,21,27 ,44,57,55 ,56,57,54 },
	{22,27,28 ,42,55,58 ,44,58,59 }, {23,22,28 ,45,44,59 ,46,44,59 },
	{23,28,29 ,45,59,60 ,46,59,60 }, {30,24,29 ,61,46,62 ,61,47,62 },
	{29,24,23 ,62,46,47 ,62,47,48 }, {19,24,26 ,49,50,63 ,40,49,51 },
	{26,24,30 ,63,50,64 ,51,49,63 }, {31,25,26 ,65,54,66 ,64,53,65 },
	{31,27,25 ,65,55,54 ,64,54,53 }, {31,28,27 ,65,58,55 ,64,66,54 },
	{31,29,28 ,65,62,58 ,64,67,66 }, {31,30,29 ,65,67,62 ,64,68,67 },
	{31,26,30 ,65,66,67 ,64,65,68 }
};
static GLfloat rock_verticies [32][3] = {
{-0.00573801f,0.036247f,0.5f},{0.111271f,0.290952f,0.380973f},{0.224938f,0.043906f,0.443636f},
{-0.127084f,0.289954f,0.382932f},{-0.246925f,0.044223f,0.439019f},{-0.125462f,-0.215873f,0.467498f},
{0.110932f,-0.210139f,0.46814f},{0.373651f,0.045276f,0.318856f},{0.19352f,0.4013f,0.038895f},
{-0.264044f,0.379563f,0.090107f},{-0.429592f,0.051416f,0.266305f},{-0.214201f,-0.466721f,0.335567f},
{0.19506f,-0.311852f,0.334446f},{0.5f,0.039285f,0.027851f},{0.176992f,0.458243f,-0.054211f},
{-0.391932f,0.416277f,0.045756f},{-0.5f,0.052198f,0.03183f},{-0.246619f,-0.5f,0.027851f},
{0.226992f,-0.373022f,0.029841f},{0.362013f,0.043755f,-0.185401f},{0.152799f,0.5f,-0.282607f},
{-0.307316f,0.478396f,-0.230371f},{-0.43288f,0.050196f,-0.188291f},{-0.215829f,-0.428962f,-0.271824f},
{0.195984f,-0.274225f,-0.273969f},{0.141822f,0.43003f,-0.345113f},{0.274486f,0.019347f,-0.388344f},
{-0.15238f,0.431623f,-0.342648f},{-0.295959f,0.024268f,-0.389713f},{-0.156387f,-0.240721f,-0.438201f},
{0.135854f,-0.237577f,-0.438066f},{0.00676198f,0.023334f,-0.5f}
};
static GLfloat rock_normals [68][3] = {
{0.44066f,0.145726f,0.885767f},{0.396592f,0.491034f,0.775629f},{-0.00180538f,0.160207f,0.987082f},
{-0.109222f,0.372926f,0.92141f},{-0.468481f,0.139256f,0.872429f},{-0.250739f,-0.342775f,0.905337f},
{0.120136f,-0.0711555f,0.990204f},{0.675991f,0.57267f,0.463773f},{0.719474f,0.6227f,0.307573f},
{-0.00146464f,0.95181f,0.306684f},{0.00136128f,0.926199f,0.377032f},{-0.0067445f,0.953215f,0.302218f},
{-0.0250739f,0.816792f,0.576388f},{-0.63167f,0.519911f,0.575052f},{-0.624407f,0.489606f,0.608606f},
{-0.649329f,0.587129f,0.483375f},{-0.763692f,-0.255976f,0.592664f},{-0.855547f,-0.321936f,0.405458f},
{0.19753f,-0.78247f,0.590528f},{0.017509f,-0.790398f,0.612344f},{0.195184f,-0.509732f,0.8379f},
{0.77359f,-0.382621f,0.505134f},{0.656248f,-0.261286f,0.707861f},{0.811452f,-0.386657f,0.438225f},
{0.737562f,0.615974f,0.276726f},{0.842165f,0.532443f,-0.0852206f},{-0.00105304f,0.926358f,0.376642f},
{-0.0485994f,0.819601f,0.57087f},{-0.957916f,0.2754f,-0.0809453f},{-0.958637f,0.284625f,-0.00201582f},
{-0.91735f,-0.394962f,0.0497278f},{-0.908779f,-0.404089f,-0.104081f},{0.304983f,-0.950626f,-0.0574084f},
{0.303015f,-0.941298f,0.148792f},{0.258715f,-0.964013f,-0.0611961f},{0.871697f,-0.487866f,-0.046172f},
{0.781804f,-0.51598f,0.350067f},{0.741291f,0.479537f,-0.469609f},{0.855465f,0.382767f,-0.34881f},
{0.899134f,0.358948f,-0.250429f},{-0.0254434f,0.982896f,0.182395f},{-0.0304051f,0.980187f,0.195726f},
{-0.884526f,0.293516f,-0.362577f},{-0.939529f,0.2528f,-0.231037f},{-0.84171f,-0.25846f,-0.474049f},
{-0.83925f,-0.320706f,-0.439099f},{0.206211f,-0.881422f,-0.424938f},{0.288126f,-0.863875f,-0.413162f},
{0.729402f,-0.485298f,-0.48214f},{0.837609f,-0.420613f,-0.348562f},{0.868654f,-0.381572f,-0.315978f},
{0.847175f,0.30225f,-0.436965f},{0.855613f,0.319278f,-0.407416f},{-0.0503278f,0.776333f,-0.62831f},
{0.128193f,0.430008f,-0.893678f},{-0.325173f,0.466038f,-0.822843f},{-0.0966033f,0.864415f,-0.49341f},
{-0.567481f,0.0859325f,-0.81889f},{-0.501428f,0.191204f,-0.843807f},{-0.782193f,-0.282704f,-0.555205f},
{-0.751706f,-0.287307f,-0.593627f},{0.0105924f,-0.97507f,-0.221645f},{-0.02214f,-0.558351f,-0.82931f},
{0.855611f,-0.369871f,-0.362113f},{0.839272f,-0.377045f,-0.39174f},{0.0150675f,0.0946363f,-0.995398f},
{0.384391f,0.0947983f,-0.91829f},{0.198386f,-0.13237f,-0.971144f}
};
static GLfloat rock_textures [69][2] = {
{0.275062f,0.456094f},{0.388729f,0.209048f},{0.505738f,0.463753f},
{0.627084f,0.210046f},{0.746925f,0.455777f},{0.625462f,0.715873f},
{0.389068f,0.710139f},{0.126349f,0.454724f},{0.790952f,0.880973f},
{0.545276f,0.818856f},{0.9013f,0.538895f},{0.388729f,0.880973f},
{0.30648f,0.538895f},{0.627084f,0.882932f},{0.764044f,0.590107f},
{0.929592f,0.448584f},{0.448584f,0.766305f},{0.210046f,0.882932f},
{0.120437f,0.590107f},{0.714201f,0.966721f},{0.69506f,0.834446f},
{0.610932f,0.96814f},{0.374538f,0.967498f},{0.30494f,0.811852f},
{0.289861f,0.96814f},{0.188148f,0.834446f},{0.539285f,0.527851f},
{0.958243f,0.445789f},{0.323008f,0.445789f},{0.891932f,0.545756f},
{0.764044f,0.120437f},{0.891932f,0.083723f},{0.083723f,0.545756f},
{0.447802f,0.53183f},{0.966721f,0.835567f},{1.0f,0.527851f},
{0.726992f,0.529841f},{0.285799f,0.835567f},{0.253381f,0.527851f},
{0.126978f,0.529841f},{0.543755f,0.314599f},{1.0f,0.217393f},
{0.347201f,0.217393f},{0.807316f,0.269629f},{0.449804f,0.311709f},
{0.021604f,0.269629f},{0.928962f,0.228176f},{0.695984f,0.226031f},
{0.284171f,0.228176f},{0.225775f,0.226031f},{0.93003f,0.154887f},
{0.519347f,0.111656f},{0.347201f,1.0f},{0.358178f,0.93003f},
{0.65238f,0.931623f},{0.65238f,0.157352f},{0.93288f,0.550196f},
{0.807316f,0.978396f},{0.068377f,0.157352f},{0.475732f,0.110287f},
{0.740721f,0.061799f},{0.635854f,0.061934f},{0.343613f,0.061799f},
{0.262423f,0.061934f},{0.493238f,0.523334f},{0.225514f,0.519347f},
{0.795959f,0.524268f},{0.656387f,0.259279f},{0.364146f,0.262423f}
};


// UFO object

// 24 Verticies
// 54 Texture Coordinates
// 39 Normals
// 44 Triangles

static BYTE ufo_face_indicies[44][9] = {
	{5,4,3 ,0,0,0 ,0,1,2 }, {5,3,0 ,0,0,0 ,0,2,3 }, {0,3,2 ,0,0,0 ,3,4,5 },
	{0,2,1 ,0,0,0 ,3,5,6 }, {5,0,18 ,1,2,1 ,7,8,9 }, {5,18,19 ,1,1,3 ,7,9,10 },
	{5,19,4 ,4,5,5 ,7,10,11 }, {4,19,20 ,5,5,6 ,11,10,12 }, {3,4,20 ,7,7,7 ,13,11,12 },
	{3,20,21 ,7,7,8 ,13,12,14 }, {2,3,21 ,9,10,9 ,15,16,17 }, {2,21,22 ,9,9,11 ,15,17,18 },
	{23,1,2 ,12,13,12 ,19,20,15 }, {23,2,22 ,12,12,14 ,19,15,18 },
	{0,1,23 ,15,15,15 ,8,20,19 }, {0,23,18 ,15,15,15 ,8,19,9 }, {8,7,9 ,16,17,18 ,21,22,23 },
	{9,7,6 ,18,17,19 ,23,22,24 }, {9,6,11 ,18,19,20 ,25,26,27 },
	{9,11,10 ,18,20,21 ,25,27,28 }, {7,13,6 ,17,22,19 ,22,29,30 },
	{6,13,12 ,19,22,23 ,30,29,31 }, {14,13,8 ,24,22,16 ,32,29,21 },
	{8,13,7 ,16,22,17 ,21,29,22 }, {14,8,15 ,24,16,25 ,32,21,33 },
	{15,8,9 ,25,16,18 ,33,21,23 }, {15,9,10 ,25,18,21 ,34,25,28 },
	{15,10,16 ,25,21,26 ,34,28,35 }, {10,11,16 ,21,20,26 ,28,27,35 },
	{16,11,17 ,26,20,27 ,35,27,36 }, {6,12,11 ,19,23,20 ,26,37,27 },
	{11,12,17 ,20,23,27 ,27,37,36 }, {13,19,18 ,28,29,30 ,38,39,40 },
	{13,18,12 ,28,30,31 ,38,40,41 }, {13,14,20 ,28,32,33 ,38,42,43 },
	{13,20,19 ,28,33,29 ,38,43,39 }, {20,14,15 ,33,32,34 ,43,42,44 },
	{20,15,21 ,33,34,34 ,43,44,45 }, {21,15,22 ,34,34,35 ,46,47,48 },
	{22,15,16 ,35,34,36 ,48,47,49 }, {23,22,16 ,37,35,36 ,50,48,49 },
	{23,16,17 ,37,36,38 ,50,49,51 }, {12,18,23 ,31,30,37 ,52,53,50 },
	{12,23,17 ,31,37,38 ,52,50,51 }
};
static GLfloat ufo_verticies [24][3] = {
{0.067294f,-9.01417e-09f,0.125f},{0.033647f,-0.067294f,0.125f},{-0.033647f,-0.067294f,0.125f},
{-0.0672939f,3.32918e-09f,0.125f},{-0.0336469f,0.067294f,0.125f},{0.033647f,0.0672939f,0.125f},
{0.167449f,-1.47585e-08f,-0.125f},{0.0837245f,0.167449f,-0.125f},{-0.0837244f,0.167449f,-0.125f},
{-0.167449f,1.59557e-08f,-0.125f},{-0.0837245f,-0.167449f,-0.125f},{0.0837245f,-0.167449f,-0.125f},
{0.5f,-3.38318e-08f,-0.0416667f},{0.25f,0.5f,-0.0416667f},{-0.25f,0.5f,-0.0416667f},
{-0.5f,5.78803e-08f,-0.0416667f},{-0.25f,-0.5f,-0.0416667f},{0.25f,-0.5f,-0.0416667f},
{0.167449f,-1.47585e-08f,0.0416667f},{0.0837245f,0.167449f,0.0416667f},{-0.0837244f,0.167449f,0.0416667f},
{-0.167449f,1.59557e-08f,0.0416667f},{-0.0837245f,-0.167449f,0.0416667f},{0.0837245f,-0.167449f,0.0416667f}
};
static GLfloat ufo_normals [39][3] = {
{0.0f,0.0f,1.0f},{0.609205f,0.304603f,0.732179f},{0.609205f,0.304603f,0.732179f},
{0.609205f,0.304603f,0.732179f},{2.12443e-07f,0.639599f,0.768708f},{2.20057e-07f,0.639599f,0.768708f},
{2.2767e-07f,0.639599f,0.768708f},{-0.609205f,0.304603f,0.732179f},{-0.609205f,0.304603f,0.732179f},
{-0.609205f,-0.304603f,0.732179f},{-0.609205f,-0.304603f,0.732179f},{-0.609205f,-0.304603f,0.732179f},
{-3.54072e-08f,-0.639599f,0.768708f},{-7.08145e-08f,-0.639599f,0.768708f},{0.0f,-0.639599f,0.768708f},
{0.609205f,-0.304603f,0.732179f},{-0.0976209f,0.14715f,-0.984285f},{0.0609786f,0.0919166f,-0.993898f},
{-0.0811701f,8.77208e-09f,-0.9967f},{0.146432f,0.0244052f,-0.98892f},{0.0813754f,-0.122662f,-0.989107f},
{-0.122009f,-0.122458f,-0.984946f},{0.121769f,0.183549f,-0.975439f},{0.242873f,-0.0404789f,-0.969213f},
{-0.121769f,0.183549f,-0.975439f},{-0.243072f,2.62689e-08f,-0.970008f},{-0.0810952f,-0.203931f,-0.975621f},
{0.121769f,-0.183549f,-0.975439f},{0.121769f,0.183549f,0.975439f},{0.121769f,0.183549f,0.975439f},
{0.242873f,0.0404788f,0.969213f},{0.242873f,-0.0404789f,0.969213f},{-0.121769f,0.183549f,0.975439f},
{-0.121769f,0.183549f,0.975439f},{-0.243072f,2.62689e-08f,0.970008f},{-0.16219f,-0.162787f,0.973239f},
{-0.0810952f,-0.203931f,0.975621f},{0.121769f,-0.183549f,0.975439f},{0.121769f,-0.183549f,0.975439f}
};
static GLfloat ufo_textures [54][2] = {
{0.710208f,0.481f},{0.857792f,0.481f},{1.034f,0.481f},
{0.534f,0.481f},{0.034f,0.481f},{0.210208f,0.481f},
{0.357792f,0.481f},{0.710208f,0.141f},{0.534f,0.141f},
{0.534f,0.137667f},{0.710208f,0.137667f},{0.857792f,0.141f},
{0.857792f,0.137667f},{1.034f,0.141f},{1.034f,0.137667f},
{0.210208f,0.141f},{0.0339999f,0.141f},{0.0339999f,0.137667f},
{0.210208f,0.137667f},{0.357792f,0.137667f},{0.357792f,0.141f},
{0.857792f,0.71f},{0.710208f,0.71f},{1.034f,0.71f},
{1.534f,0.71f},{0.0339999f,0.71f},{0.534f,0.71f},
{0.357792f,0.71f},{0.210208f,0.71f},{0.710208f,0.713333f},
{0.534f,0.71f},{0.534f,0.713333f},{0.857792f,0.713333f},
{1.034f,0.713333f},{0.0339999f,0.713333f},{0.210208f,0.713333f},
{0.357792f,0.713333f},{0.534f,0.713333f},{0.710208f,0.474333f},
{0.710208f,0.477667f},{0.534f,0.477667f},{0.534f,0.474333f},
{0.857792f,0.474333f},{0.857792f,0.477667f},{1.034f,0.474333f},
{1.034f,0.477667f},{0.0339999f,0.477667f},{0.0339999f,0.474333f},
{0.210208f,0.477667f},{0.210208f,0.474333f},{0.357792f,0.477667f},
{0.357792f,0.474333f},{0.534f,0.474333f},{0.534f,0.477667f}
};