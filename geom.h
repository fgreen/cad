#ifndef __IS_GEOM__
#define __IS_GEOM__

#include "geom-interface.h"
#include "expr.h"
#include "octree.h"

class ArgsGeom : public Geom {
 public:
  std::vector<Geom*> val;
 ArgsGeom(void) : Geom(args_kind) { }
};

class NumGeom : public Geom {
 public:
  double val;
 NumGeom(double val) : Geom(num_kind), val(val) { }
};

class StringGeom : public Geom {
 public:
  std::string val;
 StringGeom(std::string val) : Geom(string_kind), val(val) { }
};

class V2dGeom : public Geom {
 public:
  TV2 val;
 V2dGeom(TV2 val) : Geom(v2d_kind), val(val) { }
};

class V3dGeom : public Geom {
 public:
  TV3 val;
 V3dGeom(TV3 val) : Geom(v3d_kind), val(val) { }
};

class CircleArcGeom : public Geom {
 public:
  CircleArc val;
 CircleArcGeom(CircleArc val) : Geom(circle_arc_kind), val(val) { }
};

class V3iGeom : public Geom {
 public:
  IV3 val;
 V3iGeom(IV3 val) : Geom(v3i_kind), val(val) { }
};

class MatGeom : public Geom {
 public:
  Matrix<T,4> val;
 MatGeom(Matrix<T,4> val) : Geom(mat_kind), val(val) { }
};

class ArrayV2dGeom : public Geom {
 public:
  Array<TV2> val;
 ArrayV2dGeom(Array<TV2> val) : Geom(array_v2d_kind), val(val) { }
};

class ArrayV3dGeom : public Geom {
 public:
  Array<TV3> val;
 ArrayV3dGeom(Array<TV3> val) : Geom(array_v3d_kind), val(val) { }
};

class ArrayV3iGeom : public Geom {
 public:
  Array<IV3> val;
 ArrayV3iGeom(Array<IV3> val) : Geom(array_v3i_kind), val(val) { }
};

class ArrayCircleArcGeom : public Geom {
 public:
  Array<CircleArc> val;
 ArrayCircleArcGeom(Array<CircleArc> val) : Geom(array_circle_arc_kind), val(val) { }
};

class NestedV2dGeom : public Geom {
 public:
  Nested<TV2> val;
 NestedV2dGeom(Nested<TV2> val) : Geom(nested_v2d_kind), val(val) { }
};

class NestedV3dGeom : public Geom {
 public:
  Nested<TV3> val;
 NestedV3dGeom(Nested<TV3> val) : Geom(nested_v3d_kind), val(val) { }
};

class NestedCircleArcGeom : public Geom {
 public:
  Nested<CircleArc> val;
 NestedCircleArcGeom(Nested<CircleArc> val) : Geom(nested_circle_arc_kind), val(val) { }
};

class PolyGeom : public Geom {
 public:
  Nested<TV2> val;
 PolyGeom(Nested<TV2> val) : Geom(poly_kind), val(val) { }
};

class MeshGeom : public Geom {
 public:
  Mesh val;
 MeshGeom(Tuple<Ref<TriangleSoup>, Array<TV3>> val) : Geom(mesh_kind), val(const_mesh(val)) { }
 MeshGeom(Mesh val) : Geom(mesh_kind), val(val) { }
};

extern "C" int g_kind(Geom* g);

extern bool all_args_kind (std::vector<Geom*> args, int kind);
extern Geom* g_circle_arc (CircleArc v);
extern Geom* g_array_v2d (std::vector<Geom*> args);
extern Geom* g_array_v3d (std::vector<Geom*> args);
extern Geom* g_array_v3i (std::vector<Geom*> args);
extern Geom* g_array_circle_arc(Array<CircleArc> arcs);
extern Geom* g_nested_v2d (std::vector<Geom*> args);
extern Geom* g_nested_v3d (std::vector<Geom*> args);
extern Geom* g_nested_circle_arc(Nested<CircleArc> arcz);
extern Geom* g_arcs (std::vector<Geom*> args);
extern Geom* g_poly (std::vector<Geom*> args);
extern Geom* g_circle_arcs (std::vector<Geom*> args);

extern "C" Geom* g_args_fab(void);
extern "C" Geom* g_args_add(Geom* g, Geom* e);
extern "C" int g_args_len(Geom* g);
extern "C" int g_args_free(Geom* g);
extern "C" Geom* g_num_fab(float a);
extern "C" float g_num_value(Geom* g);
extern "C" Geom* g_string_fab(char* str);
extern "C" int g_string_len(Geom* g);
extern "C" char* g_string_c_str(Geom*);
extern "C" Geom* g_circle_arc_fab(float x, float y, float q);
extern "C" float g_circle_arc_x(Geom* g);
extern "C" float g_circle_arc_y(Geom* g);
extern "C" float g_circle_arc_q(Geom* g);
extern "C" Geom* g_v2d_fab(float x, float y);
extern "C" float g_v2d_elt(Geom* g, int idx);
extern "C" float g_v2d_x(Geom* g);
extern "C" float g_v2d_y(Geom* g);
extern "C" Geom* g_v3d_fab(float x, float y, float z);
extern "C" float g_v3d_elt(Geom* g, int idx);
extern "C" float g_v3d_x(Geom* g);
extern "C" float g_v3d_y(Geom* g);
extern "C" float g_v3d_z(Geom* g);
extern "C" Geom* g_v3i_fab(int x, int y, int z);
extern "C" int g_v3i_elt(Geom* g, int idx);
extern "C" int g_v3i_x(Geom* g);
extern "C" int g_v3i_y(Geom* g);
extern "C" int g_v3i_z(Geom* g);
extern "C" Geom* g_bbox2_min(Geom* g);
extern "C" Geom* g_bbox2_max(Geom* g);
extern "C" Geom* g_bbox3_min(Geom* g);
extern "C" Geom* g_bbox3_max(Geom* g);
extern "C" Geom* g_mat_fab(float i00, float i01, float i02, float i03, float i10, float i11, float i12, float i13,
                           float i20, float i21, float i22, float i23, float i30, float i31, float i32, float i33);
extern "C" float g_mat_elt(Geom* g, int i, int j);
extern "C" Geom* g_array_v2d_fab(Geom* args);
extern "C" Geom* g_array_v2d_elt(Geom* g, int idx);
extern "C" int g_array_v2d_len(Geom* g);
extern "C" Geom* g_array_v3d_fab(Geom* args);
extern "C" Geom* g_array_v3d_elt(Geom* g, int idx);
extern "C" int g_array_v3d_len(Geom* g);
extern "C" Geom* g_array_v3i_fab(Geom* args);
extern "C" Geom* g_array_v3i_elt(Geom* g, int idx);
extern "C" int g_array_v3i_len(Geom* g);
extern "C" Geom* g_array_circle_arc_fab(Geom* args);
extern "C" Geom* g_array_circle_arc_elt(Geom* g, int idx);
extern "C" int g_array_circle_arc_len(Geom* g);
extern "C" Geom* g_nested_v2d_fab(Geom* args);
extern "C" Geom* g_nested_v2d_elt(Geom* g, int idx);
extern "C" int g_nested_v2d_len(Geom* g);
extern "C" Geom* g_nested_v3d_fab(Geom* args);
extern "C" Geom* g_nested_v3d_elt(Geom* g, int idx);
extern "C" int g_nested_v3d_len(Geom* g);
extern "C" Geom* g_nested_circle_arc_fab(Geom* args);
extern "C" Geom* g_nested_circle_arc_elt(Geom* g, int idx);
extern "C" int g_nested_circle_arc_len(Geom* g);
extern "C" Geom* g_poly_fab(Geom* args);
extern "C" Geom* g_poly_elt(Geom* g, int idx);
extern "C" int g_poly_len(Geom* g);
extern "C" Geom* g_mesh_fab(Geom* points, Geom* faces);
extern "C" Geom* g_mesh_points(Geom* g);
extern "C" Geom* g_mesh_faces(Geom* g);
extern "C" Geom* g_mesh_bounds(Geom* g);
extern "C" Geom* g_mesh_load(Geom* g);
extern "C" Geom* g_bbox(Geom* g);
extern "C" Geom* g_dims(Geom* g);
extern "C" Geom* g_center(Geom* g);
extern "C" Geom* g_centering(Geom* g);
extern "C" Geom* g_load(Geom* s);
extern "C" Geom* g_save(Geom* s, Geom* g);
extern "C" char* g_c_str(Geom* g);
extern "C" Geom* g_to_str(Geom* g);
extern "C" Geom* g_print(Geom* g);
extern "C" Geom* g_check(Geom* g);
extern "C" Geom* g_pretty_print(Geom* g);
extern "C" Geom* g_pi(void);
extern "C" Geom* g_none2(void);
extern "C" Geom* g_all2(void);
extern "C" Geom* g_none3(void);
extern "C" Geom* g_all3(void);
extern "C" Geom* g_circle(Geom* a);
extern "C" Geom* g_square(Geom* a);
extern "C" Geom* g_square_lo_hi(Geom* lo, Geom* hi);
extern "C" Geom* g_letter(Geom* a);
extern "C" Geom* g_text(Geom* a);
extern "C" Geom* g_elt(Geom* g, Geom* i);
extern "C" Geom* g_mag(Geom* v, Geom* g);
extern "C" Geom* g_mag1(Geom* a, Geom* g);
extern "C" Geom* g_xmag(Geom* a, Geom* g);
extern "C" Geom* g_ymag(Geom* a, Geom* g);
extern "C" Geom* g_zmag(Geom* a, Geom* g);
extern "C" Geom* g_mov(Geom* v, Geom* g);
extern "C" Geom* g_xmov(Geom* a, Geom* g);
extern "C" Geom* g_ymov(Geom* a, Geom* g);
extern "C" Geom* g_zmov(Geom* a, Geom* g);
extern "C" Geom* g_rot(Geom* v, Geom* g);
extern "C" Geom* g_rot_from_to(Geom* from, Geom* to, Geom* g);
extern "C" Geom* g_xrot(Geom* a, Geom* g);
extern "C" Geom* g_yrot(Geom* a, Geom* g);
extern "C" Geom* g_zrot(Geom* a, Geom* g);
extern "C" Geom* g_reflect_x(Geom* g);
extern "C" Geom* g_reflect_y(Geom* g);
extern "C" Geom* g_reflect_xy(Geom* g);
extern "C" Geom* g_reflect_z(Geom* g);
extern "C" Geom* g_reflect_yz(Geom* g);
extern "C" Geom* g_reflect_xz(Geom* g);
extern "C" Geom* g_add(Geom* a, Geom* b);
extern "C" Geom* g_magnitude(Geom* a);
extern "C" Geom* g_normalize(Geom* a);
extern "C" Geom* g_cross(Geom* a, Geom* b);
extern "C" Geom* g_dot(Geom* a, Geom* b);
extern "C" Geom* g_mul(Geom* a, Geom* b);
extern "C" Geom* g_div(Geom* a, Geom* b);
extern "C" Geom* g_dither(Geom* a);
extern "C" Geom* g_sub(Geom* a, Geom* b);
extern "C" Geom* g_intersection(Geom* a, Geom* b);
extern "C" Geom* g_union(Geom* a, Geom* b);
extern "C" Geom* g_difference(Geom* a, Geom* b);
extern "C" Geom* g_not(Geom* a);
extern "C" Geom* g_offset(Geom* a, Geom* g);
extern "C" Geom* g_open_offset(Geom* a, Geom* g);
extern "C" Geom* g_hollow(Geom* a, Geom* m);
extern "C" Geom* g_simplify(Geom* g);
extern "C" Geom* g_cleanup(Geom* g);
extern "C" Geom* g_slice(Geom* a, Geom* g);
extern "C" Geom* g_extrude(Geom* a, Geom* p);
extern "C" Geom* g_thicken(Geom* a, Geom* l);
extern "C" Geom* g_sphere(Geom* a);
extern "C" Geom* g_cube(Geom* a);
extern "C" Geom* g_cube_lo_hi(Geom* lo, Geom* hi);
extern "C" Geom* g_cone(Geom* a, Geom* p);
extern "C" Geom* g_revolve(Geom* p);
extern "C" Geom* g_hull(Geom* m);
extern "C" Geom* g_taper(Geom* l, Geom* r0, Geom* r1, Geom* p);
extern "C" Geom* g_to_mesh(Geom* g);
extern "C" Geom* g_to_tree(Geom* g, Geom* rad, Geom* thresh);

/// NEW API

extern "C" Geom* g_mesh_all(void);
extern "C" Geom* g_mesh_none(void);
extern "C" Geom* g_mesh_save(Geom* s, Geom* g);
extern "C" Geom* g_mesh_load(Geom* s);
extern "C" Geom* g_mesh_mul(Geom* m, Geom* g);
extern "C" Geom* g_mesh_intersection(Geom* a, Geom* b);
extern "C" Geom* g_mesh_union(Geom* a, Geom* b);
extern "C" Geom* g_mesh_difference(Geom* a, Geom* b);
extern "C" Geom* g_mesh_not(Geom* a);
extern "C" Geom* g_mesh_offset(float r, Geom* g);
extern "C" Geom* g_mesh_offset_rough(float r, Geom* g);
extern "C" Geom* g_mesh_simplify(Geom* g);
extern "C" Geom* g_mesh_cleanup(Geom* g);
extern "C" Geom* g_mesh_slice(float z, Geom* g);
extern "C" Geom* g_mesh_hull(Geom* m);

extern "C" int g_polygon_len(Geom* g);
extern "C" Geom* g_polygon_fab(Geom* g);
extern "C" Geom* g_polygon_elt(Geom* g, int i);
extern "C" Geom* g_polygon_bounds(Geom* g);
extern "C" Geom* g_polygon_all(void);
extern "C" Geom* g_polygon_none(void);
extern "C" Geom* g_polygon_save(Geom* s, Geom* g);
extern "C" Geom* g_polygon_load(Geom* s);
extern "C" Geom* g_polygon_revolve(Geom* s);
extern "C" Geom* g_polygon_square(float d);
extern "C" Geom* g_polygon_square_lo_hi(Geom* lo, Geom* hi);
extern "C" Geom* g_polygon_circle(float d);
extern "C" Geom* g_polygon_mul(Geom* m, Geom* g);
extern "C" Geom* g_polygon_intersection(Geom* a, Geom* b);
extern "C" Geom* g_polygon_union(Geom* a, Geom* b);
extern "C" Geom* g_polygon_difference(Geom* a, Geom* b);
extern "C" Geom* g_polygon_not(Geom* a);
extern "C" Geom* g_polygon_offset(float r, Geom* g);
extern "C" Geom* g_polygon_hull(Geom* m);
extern "C" Geom* g_polygon_thicken(float d, Geom* m);

extern "C" int   g_nested_circle_arc_len(Geom* g);
extern "C" Geom* g_nested_circle_arc_fab(Geom* g);
extern "C" Geom* g_nested_circle_arc_elt(Geom* g, int i);
extern "C" Geom* g_nested_circle_arc_open_offset(float d, Geom* p);
extern "C" Geom* g_nested_circle_arc_closed_offset(float d, Geom* p);
extern "C" Geom* g_nested_circle_arc_discretize(float max_deviation, int is_closed, Geom* p);
extern "C" Geom* g_nested_circle_arc_union(Geom* a, Geom* b);
extern "C" Geom* g_nested_circle_arc_intersection(Geom* a, Geom* b);
extern "C" Geom* g_nested_circle_arc_intersection1(Geom* a);
extern "C" Geom* g_nested_circle_arc_concat(Geom* a, Geom* b);
extern "C" Geom* g_nested_circle_arc_not(Geom* a);
extern "C" Geom* g_nested_circle_arc_difference(Geom* a, Geom* b);
extern "C" Geom* g_nested_circle_arc_bounds(Geom* g);
extern "C" Geom* g_nested_circle_arc_find_overlapping_offsets(float d, Geom* a);

extern "C" Geom* g_mesh_extrude(float d, Geom* p);
extern "C" Geom* g_mesh_thicken(float d, Geom* l);
extern "C" Geom* g_mesh_sphere(float d);
extern "C" Geom* g_mesh_cube(float d);
extern "C" Geom* g_mesh_cube_lo_hi(Geom* lo, Geom* hi);
extern "C" Geom* g_mesh_cone(float h, Geom* p);
extern "C" Geom* g_mesh_revolve(Geom* p);


extern std::vector<Geom*> g_args_val(Geom* g);
extern Geom* g_num(T a);
extern T g_num_val(Geom* g);
extern Geom* g_string(std::string s);
extern std::string g_string_val(Geom* g);

extern Geom* g_v2d(TV2 v);
extern TV2 g_v2d_val(Geom* g);

extern Geom* g_v3d(TV3 v);
extern TV3 g_v3d_val(Geom* g);

extern Geom* g_v3i(IV3 v);
extern IV3 g_v3i_val(Geom* g);

extern Geom* g_mat(Matrix<T,4> mat);
extern Matrix<T,4> g_mat_val(Geom* g);

extern Geom* g_array_v2d(Array<TV2> line);
extern Geom* g_array_v2d(RawArray<TV2> line);
extern Array<TV2> g_array_v2d_val(Geom* g);

extern Geom* g_array_v3d(Array<TV3> line);
extern Geom* g_array_v3d(RawArray<TV3> line);
extern Array<TV3> g_array_v3d_val(Geom* g);

extern Geom* g_array_circle_arc(Array<CircleArc> arcs);
extern Geom* g_array_circle_arc(RawArray<CircleArc> arcs);
extern Array<CircleArc> g_array_circle_arc_val(Geom* g);

extern Geom* g_array_v3i(Array<IV3> a);
extern Array<IV3> g_array_v3i_val(Geom* g);

extern bool is_nested_v2d(Geom* g);
extern Geom* g_nested_v2d(Nested<TV2> polyline);
extern Nested<TV2> g_nested_v2d_val(Geom* g);

extern Geom* g_nested_v3d(Nested<TV3> polyline);
extern bool is_nested_v3d(Geom* g);
extern Nested<TV3> g_nested_v3d_val(Geom* g);

extern Geom* g_nested_circle_arc(Nested<CircleArc> arcs);
extern Geom* g_arcs(Nested<TV2> arcz);
extern Nested<CircleArc> g_nested_circle_arc_val(Geom* g);

extern bool is_poly(Geom* g);
extern Geom* g_poly(Nested<TV2> poly);
extern Nested<TV2> g_poly_val(Geom* g);

extern Geom* g_mesh(Mesh mesh);
extern Mesh g_mesh_val(Geom* g);
extern std::string g_to_str_val(Geom* g);

/// EXPR GEOMS

extern Geom* g_to_expr(Geom* g);
extern Expr* g_expr_val(Geom* g);
extern Geom* g_expr(Geom* g);
extern Geom* g_lit(flo_t a);
extern Geom* g_x(void);
extern Geom* g_y(void);
extern Geom* g_z(void);
extern Geom* g_neg(Geom* g);
extern Geom* g_min (Geom* a, Geom* b);
extern Geom* g_or (Geom* a, Geom* b);
extern Geom* g_max (Geom* a, Geom* b);
extern Geom* g_and (Geom* a, Geom* b);
extern Geom* g_pow(Geom* a, Geom* b);
extern Geom* g_sqr(Geom* a);
extern Geom* g_abs(Geom* a);
extern Geom* g_sqrt(Geom* a);
extern Geom* g_sin(Geom* g);
extern Geom* g_cos(Geom* g);
extern Geom* g_tan(Geom* a);
extern Geom* g_asin(Geom* g);
extern Geom* g_acos(Geom* g);
extern Geom* g_atan(Geom* a);
extern Geom* g_xform(Geom* nx, Geom* ny, Geom* nz, Geom* g);
extern Geom* g_half(Geom* nx, Geom* ny, Geom* nz, Geom* d);
extern Geom* g_edge(Geom* x0, Geom* y0, Geom* x1, Geom* y1);
extern Geom* g_triangle(Geom* x0, Geom* y0, Geom* x1, Geom* y1, Geom* x2, Geom* y2);
extern Geom* g_cylinder(Geom* r, Geom* lo, Geom* hi);
extern Geom* g_capsule(Geom* r, Geom* lo, Geom* hi);
extern Geom* g_pyramid(Geom* xyd, Geom* zd);
extern Geom* g_shear(Geom* a, Geom* b, Geom* c, Geom* d, Geom* e);
extern Geom* g_blend(Geom* a, Geom* b, Geom* c);

extern Geom* g_space(Geom* r);
extern Geom* g_xbox(Geom* a, Geom* b);
extern Geom* g_ybox(Geom* a, Geom* b);
extern Geom* g_zbox(Geom* a, Geom* b);
extern Geom* g_align_xmin(Geom* a, Geom* b);
extern Geom* g_align_ymin(Geom* a, Geom* b);
extern Geom* g_align_zmin(Geom* a, Geom* b);
extern Geom* g_align_xmax(Geom* a, Geom* b);
extern Geom* g_align_ymax(Geom* a, Geom* b);
extern Geom* g_align_zmax(Geom* a, Geom* b);

#endif
