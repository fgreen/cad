#include "cad.h"
#include "hull.h"
#include "iso-surface.h"
#include "geom.h"

#include <cstdio>
#include <math.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>

int g_kind(Geom* g) { return g->k; }

extern Mesh quick_hull(Mesh mesh);

Geom* g_args_fab(void) { return new ArgsGeom(); }
Geom* g_args_add(Geom* g, Geom* e) {
  ((ArgsGeom*)g)->val.push_back(e);
  return g;
}
int g_args_len(Geom* g) { return g_args_val(g).size(); }
std::vector<Geom*> g_args_val(Geom* g) {
  ensure(g->k == args_kind, "NOT ARGS");
  return ((ArgsGeom*)g)->val;
}
int g_args_free(Geom* g) { free(g); return 1; }

inline float int_as_float(int xi) {
  float x =  *(float*)&xi;
  return x;
}

inline int float_as_int(float xf) {
  int x =  *(int*)&xf;
  return x;
}

Geom* g_num_fab(float a) {
  return g_num((T)a);
}
double g_num_val(Geom* g) {
  ensure(g->k == num_kind, "NOT NUM");
  return ((NumGeom*)g)->val;
}

float g_num_value(Geom* g) {
  return g_num_val(g);
}

std::string g_string_val(Geom* g) {
  ensure(g->k == string_kind, "NOT STRING");
  return ((StringGeom*)g)->val;
}

Geom* g_string_fab(char* str) {
  std::string s(str);
  return g_string(s);
}

int g_string_len(Geom* g) {
  return g_string_val(g).size();
}

char* g_string_c_str(Geom* g) {
  return (char*)g_string_val(g).c_str();
}

Geom* g_v2d_fab(float x, float y) {
  auto res = g_v2d(vec((T)x, (T)y));
  return res;
}
TV2 g_v2d_val(Geom* g) {
  ensure(g->k == v2d_kind, "NOT V2D");
  return ((V2dGeom*)g)->val;
}
float g_v2d_elt(Geom* g, int idx)  { return g_v2d_val(g)[idx]; }
float g_v2d_x(Geom* g) { return g_v2d_val(g).x; }
float g_v2d_y(Geom* g) { return g_v2d_val(g).y; }

Geom* g_v3d_fab(float x, float y, float z) {
  auto res = g_v3d(vec((T)x, (T)y, (T)z));
  // printf("%p = [%f, %f, %f]\n", res, x, y, z);
  return res;
}
TV3 g_v3d_val(Geom* g) {
  ensure(g->k == v3d_kind, "NOT V3D");
  return ((V3dGeom*)g)->val;
}
float g_v3d_elt(Geom* g, int idx)  { return g_v3d_val(g)[idx]; }
float g_v3d_x(Geom* g) { return g_v3d_val(g).x; }
float g_v3d_y(Geom* g) { return g_v3d_val(g).y; }
float g_v3d_z(Geom* g) { return g_v3d_val(g).z; }

Geom* g_circle_arc_fab(float x, float y, float q) {
  CircleArc data(x, y, q);
  auto res = g_circle_arc(data);
  // printf("%p = [%f, %f, %f]\n", res, x, y, z);
  return res;
}
CircleArc g_circle_arc_val(Geom* g) {
  ensure(g->k == circle_arc_kind, "NOT CIRCLE_ARC");
  return ((CircleArcGeom*)g)->val;
}
float g_circle_arc_x(Geom* g) { return g_circle_arc_val(g).x[0]; }
float g_circle_arc_y(Geom* g) { return g_circle_arc_val(g).x[1]; }
float g_circle_arc_q(Geom* g) { return g_circle_arc_val(g).q; }

Geom* g_v3i_fab(int x, int y, int z) { return g_v3i(vec(x, y, z)); }
IV3 g_v3i_val(Geom* g) {
  ensure(g->k == v3i_kind || g->k == v3d_kind, "NOT V3I");
  if (g->k == v3d_kind) {
    auto v = g_v3d_val(g);
    return vec((int)(v.x), (int)(v.y), (int)(v.z));
  } else 
    return ((V3iGeom*)g)->val;
}
int g_v3i_elt(Geom* g, int idx)  { return g_v3i_val(g)[idx]; }
int g_v3i_x(Geom* g) { return g_v3i_val(g).x; }
int g_v3i_y(Geom* g) { return g_v3i_val(g).y; }
int g_v3i_z(Geom* g) { return g_v3i_val(g).z; }

Geom* g_mat_fab(float i00, float i01, float i02, float i03, float i10, float i11, float i12, float i13,
                float i20, float i21, float i22, float i23, float i30, float i31, float i32, float i33) {
  Matrix<T,4> m(i00, i01, i02, i03,
                i10, i11, i12, i13,
                i20, i21, i22, i23,
                i30, i31, i32, i33);
  auto res = g_mat(m);
  // printf("%p = mat(%f, %f, %f, %f,  %f, %f, %f, %f,  %f, %f, %f, %f,  %f, %f, %f, %f)\n",
  //        res, i00, i01, i02, i03, i10, i11, i12, i13, i20, i21, i22, i23, i30, i31, i32, i33);
  return res;
}
Matrix<T,4> g_mat_val(Geom* g) {
  ensure(g->k == mat_kind, "NOT MAT");
  return ((MatGeom*)g)->val;
}

float g_mat_elt(Geom* g, int i, int j) {
  return g_mat_val(g).x[i][j];
}

Array<TV2> g_array_v2d_val(Geom* g) {
  ensure(g->k == array_v2d_kind, "NOT ARRAY_V2D");
  return ((ArrayV2dGeom*)g)->val;
}

Geom* g_array_v2d_fab(Geom* args) {
  Array<TV2> v;
  for (auto arg : g_args_val(args)) 
    v.append(g_v2d_val(arg));
  return g_array_v2d(v);
}
Geom* g_array_v2d_elt(Geom* g, int idx) { return g_v2d(g_array_v2d_val(g)[idx]); }
int g_array_v2d_len(Geom* g) { return g_array_v2d_val(g).size(); }

Array<TV3> g_array_v3d_val(Geom* g) {
  ensure(g->k == array_v3d_kind, "NOT ARRAY_V3D");
  return ((ArrayV3dGeom*)g)->val;
}
Geom* g_array_v3d_fab(Geom* args) {
  Array<TV3> v;
  for (auto arg : g_args_val(args)) 
    v.append(g_v3d_val(arg));
  return g_array_v3d(v);
}
Geom* g_array_v3d_elt(Geom* g, int idx) { return g_v3d(g_array_v3d_val(g)[idx]); }
int g_array_v3d_len(Geom* g) { return g_array_v3d_val(g).size(); }

Array<CircleArc> g_array_circle_arc_val(Geom* g) {
  ensure(g->k == array_circle_arc_kind, "NOT ARRAY_CIRCLE_ARC");
  return ((ArrayCircleArcGeom*)g)->val;
}
Array<CircleArc> g_array_circle_arc(Array<TV2> args) {
  Array<CircleArc> v;
  for (auto arg : args) {
    CircleArc arc(arg.x, arg.y, 0.0);
    v.append(arc);
  }
  return v;
}
Array<CircleArc> g_array_circle_arc(RawArray<TV2> line) { return g_array_circle_arc(line.copy()); }
Geom* g_array_circle_arc_elt(Geom* g, int idx) { return g_circle_arc(g_array_circle_arc_val(g)[idx]); }
int g_array_circle_arc_len(Geom* g) { return g_array_circle_arc_val(g).size(); }

Array<IV3> g_array_v3i_val(Geom* g) {
  ensure(g->k == array_v3i_kind || g->k == array_v3d_kind, "NOT ARRAY_V3I");
  if (g->k == array_v3d_kind) {
    Array<IV3> array_v3i;
    for (auto v : g_array_v3d_val(g)) 
      array_v3i.append(vec((int)(v.x), (int)(v.y), (int)(v.z)));
    return array_v3i;
  }
  return ((ArrayV3iGeom*)g)->val;
}

Geom* g_array_v3i_fab(Geom* args) {
  Array<IV3> array_v3i;
  for (auto arg : g_args_val(args)) 
    array_v3i.append(g_v3i_val(arg));
  return g_array_v3i(array_v3i);
}
Geom* g_array_v3i_elt(Geom* g, int idx) {
  return g_v3i(g_array_v3i_val(g)[idx]);
}
int g_array_v3i_len(Geom* g) { return g_array_v3i_val(g).size(); }


bool is_nested_v2d(Geom* g) {
  return g->k == nested_v2d_kind || g->k == array_v2d_kind;
}
Nested<TV2> g_nested_v2d_val(Geom* g) {
  ensure(is_nested_v2d(g), "NOT NESTED_V3D");
  if (g->k == array_v2d_kind)
    return array_to_nested(g_array_v2d_val(g));
  else
    return ((NestedV2dGeom*)g)->val;
}

Geom* g_nested_v2d_fab(Geom* args) {
  Nested<TV2, false> v;
  for (auto arg : g_args_val(args)) 
    v.append(g_array_v2d_val(arg));
  v.freeze();
  return g_nested_v2d(v);
}
Geom* g_nested_circle_arc(Nested<TV2> args) {
  Nested<CircleArc, false> v;
  for (auto arg : args) 
    v.append(g_array_circle_arc(arg));
  v.freeze();
  return new NestedCircleArcGeom(v);
}
Geom* g_nested_v2d_elt(Geom* g, int idx) { return g_array_v2d(g_nested_v2d_val(g)[idx]); }
int g_nested_v2d_len(Geom* g) { return g_nested_v2d_val(g).size(); }

bool is_poly(Geom* g) {
  return g->k == poly_kind || g->k == array_v2d_kind;
}

Nested<TV2> g_poly_val(Geom* g) {
  ensure(is_poly(g), "NOT POLY");
  if (g->k == array_v2d_kind)
    return array_to_nested(g_array_v2d_val(g));
  else
    return ((PolyGeom*)g)->val;
}

Geom* g_polygon_fab(Geom* args) {
  Nested<TV2, false> v;
  for (auto arg : g_args_val(args)) 
    v.append(g_array_v2d_val(arg));
  v.freeze();
  return g_poly(v);
}
Geom* g_polygon_elt(Geom* g, int idx) { return g_array_v2d(g_poly_val(g)[idx]); }
int g_polygon_len(Geom* g) { return g_poly_val(g).size(); }

bool is_nested_circle_arc(Geom* g) {
  return g->k == nested_circle_arc_kind;
}

Nested<CircleArc> g_nested_circle_arc_val(Geom* g) {
  ensure(is_nested_circle_arc(g), "NOT NESTED_CIRCLE_ARC");
  return ((NestedCircleArcGeom*)g)->val;
}

Geom* g_nested_circle_arc_fab(Geom* args) {
  Nested<CircleArc, false> v;
  for (auto arg : g_args_val(args)) 
    v.append(g_array_circle_arc_val(arg));
  v.freeze();
  return g_nested_circle_arc(v);
}
Geom* g_nested_circle_arc_elt(Geom* g, int idx) { return g_array_circle_arc(g_nested_circle_arc_val(g)[idx]); }
int g_nested_circle_arc_len(Geom* g) { return g_nested_circle_arc_val(g).size(); }

bool is_nested_v3d(Geom* g) {
  return g->k == nested_v3d_kind || g->k == array_v3d_kind;
}

Nested<TV3> g_nested_v3d_val(Geom* g) {
  ensure(is_nested_v3d(g), "NOT NESTED_V3D");
  if (g->k == array_v3d_kind)
    return line_to_polyline(g_array_v3d_val(g));
  else
    return ((NestedV3dGeom*)g)->val;
}

Geom* g_nested_v3d_fab(Geom* args) {
  Nested<TV3, false> v;
  for (auto arg : g_args_val(args)) 
    v.append(g_array_v3d_val(arg));
  v.freeze();
  return g_nested_v3d(v);
}
Geom* g_nested_v3d_elt(Geom* g, int idx) { return g_array_v3d(g_nested_v3d_val(g)[idx]); }
int g_nested_v3d_len(Geom* g) { return g_nested_v3d_val(g).size(); }


Mesh g_mesh_val(Geom* g) {
  ensure(g->k == mesh_kind, "NOT MESH");
  return ((MeshGeom*)g)->val;
}

Geom* g_mesh_fab(Geom* vertices, Geom* faces) {
  return g_mesh(fab_mesh(g_array_v3i_val(faces), g_array_v3d_val(vertices)));
}
Geom* g_mesh_faces(Geom* g) {
  auto faces = g_mesh_val(g).soup->elements;
  Array<IV3> a;
  for (auto e : faces)
    a.append(vec(e.x, e.y, e.z));
  return g_array_v3i(a); 
}
Geom* g_mesh_points(Geom* g) {
  return g_array_v3d(g_mesh_val(g).points);
}

Box<TV3> bbox(Array<TV3> points) {
  return bounding_box(points);
  
}
Box<TV2> bbox(Array<TV2> points) {
  return bounding_box(points);
}
Box<TV3> bbox(Nested<TV3> points) {
  Box<TV3> box = Box<TV3>::empty_box();
  for (auto line : points)
    box.enlarge(bounding_box(line));
  return box;
}
Box<TV2> bbox(Nested<TV2> points) {
  Box<TV2> box = Box<TV2>::empty_box();
  for (auto line : points)
    box.enlarge(bounding_box(line));
  return box;
}
  
Geom* g_bbox2(Box<TV2> box) {
  Array<TV2> bb; bb.append(box.min); bb.append(box.max);
  return g_array_v2d(bb);
}
Geom* g_bbox3(Box<TV3> box) {
  Array<TV3> bb; bb.append(box.min); bb.append(box.max);
  return g_array_v3d(bb);
}

Geom* g_bbox2_min(Geom* g) { return g_array_v2d_elt(g, 0); }
Geom* g_bbox2_max(Geom* g) { return g_array_v2d_elt(g, 1); }
Geom* g_bbox3_min(Geom* g) { return g_array_v3d_elt(g, 0); }
Geom* g_bbox3_max(Geom* g) { return g_array_v3d_elt(g, 1); }

Geom* g_mesh_bounds(Geom* g) {
  return g_bbox3(bbox(g_mesh_val(g).points));
}
Geom* g_polygon_bounds(Geom* g) {
  return g_bbox2(bbox(g_poly_val(g)));
}
Geom* g_bbox(Geom* g) {
  if (g->k == mesh_kind)
    return g_mesh_bounds(g);
  else if (g->k == poly_kind)
    return g_polygon_bounds(g);
  else if (g->k == array_v2d_kind)
    return g_bbox2(bbox(g_array_v2d_val(g)));
  else if (g->k == array_v3d_kind)
    return g_bbox3(bbox(g_array_v3d_val(g)));
  else if (g->k == nested_v2d_kind)
    return g_bbox2(bbox(g_nested_v2d_val(g)));
  else if (g->k == nested_v3d_kind)
    return g_bbox3(bbox(g_nested_v3d_val(g)));
  else {
    error("BBOX UNDEFINED"); return NULL;
  }
}
Geom* g_dims(Geom* g) {
  auto bb = g_bbox(g);
  return g_sub(g_elt(bb, g_num(1.0)), g_elt(bb, g_num(0.0)));
}
Geom* g_center(Geom* g) {
  auto bb = g_bbox(g);
  return g_mul(g_num(0.5), g_add(g_elt(bb, g_num(0.0)), g_elt(bb, g_num(1.0))));
}
Geom* g_centering(Geom* g) {
  auto c = g_center(g);
  return g_mov(g_mul(g_num(-1), c), g);
}
Geom* g_polygon_load(Geom* s) {
  // TODO: LOAD SVG
  return s;
}

Geom* g_mesh_load(Geom* s) { 
  return new MeshGeom(read_soup(g_string_val(s)));
}

Geom* g_load(Geom* s) { 
  // TODO: LOAD SVG
  return g_mesh_load(s);
}

Geom* g_mesh_save (Geom* s, Geom* g) { 
  auto mesh = g_mesh_val(g);
  write_mesh(g_string_val(s), mesh.soup, mesh.points);
  return g;
}

Geom* g_polygon_save (Geom* s, Geom* g) { 
  save_polygon(g_string_val(s), g_poly_val(g));
  return g;
}

Geom* g_polyline_save (Geom* s, Geom* g) { 
  save_polyline(g_string_val(s), g_nested_v2d_val(g));
  return g;
}

Geom* g_save(Geom* s, Geom* g) { 
  // TODO: LOAD SVG
  if (g->k == mesh_kind) {
    g_mesh_save(s, g);
  } else if (g->k == poly_kind) {
    g_polygon_save(s, g);
  } else if (g->k == nested_v2d_kind) {
    g_polyline_save(s, g);
  }
  return g;
}

std::string g_to_str_val(Geom* g) { 
  if (g->k == num_kind)
    return num_to_str(g_num_val(g));
  else if (g->k == v2d_kind)
    return v2d_to_str(g_v2d_val(g));
  else if (g->k == v3d_kind)
    return v3d_to_str(g_v3d_val(g));
  else if (g->k == v3i_kind)
    return v3i_to_str(g_v3i_val(g));
  else if (g->k == mat_kind)
    return matrix_to_str(g_mat_val(g));
  else if (g->k == array_v2d_kind)
    return array_v2d_to_str(g_array_v2d_val(g));
  else if (g->k == array_v3d_kind)
    return array_v3d_to_str(g_array_v3d_val(g));
  else if (g->k == nested_v2d_kind)
    return nested_v2d_to_str(g_nested_v2d_val(g));
  else if (g->k == nested_v3d_kind)
    return nested_v3d_to_str(g_nested_v3d_val(g));
  else if (g->k == poly_kind)
    return poly_to_str(g_poly_val(g));
  else if (g->k == mesh_kind)
    return mesh_to_str(g_mesh_val(g));
  else
    return "-1";
}

Geom* g_to_str(Geom* g) {
  return g_string(g_to_str_val(g));
}

Geom* g_print(Geom* g) { 
  printf("%s\n", g_to_str_val(g).c_str());
  return g;
}
Geom* g_check(Geom* g) { 
  if (g->k == mesh_kind) 
    return g_mesh(check_mesh(g_mesh_val(g)));
  else {
    error("BAD CHECK"); return NULL;
  }
}

Geom* g_pretty_print(Geom* g) { 
  if (g->k == num_kind)
    pretty_print_num(g_num_val(g));
  else if (g->k == v2d_kind)
    pretty_print_v2d(g_v2d_val(g));
  else if (g->k == v3d_kind)
    pretty_print_v3d(g_v3d_val(g));
  else if (g->k == v3i_kind)
    pretty_print_v3i(g_v3i_val(g));
  else if (g->k == poly_kind)
    pretty_print_poly(g_poly_val(g));
  else if (g->k == mat_kind)
    pretty_print_matrix(g_mat_val(g));
  else if (g->k == array_v2d_kind)
    pretty_print_array_v2d(g_array_v2d_val(g));
  else if (g->k == array_v3d_kind)
    pretty_print_array_v3d(g_array_v3d_val(g));
  else if (g->k == nested_v2d_kind)
    pretty_print_nested_v2d(g_nested_v2d_val(g));
  else if (g->k == nested_v3d_kind)
    pretty_print_nested_v3d(g_nested_v3d_val(g));
  else if (g->k == mesh_kind)
    pretty_print_mesh(g_mesh_val(g));
  else if (g->k == octree_kind)
    pretty_print_tree(g_octree_val(g));
  else if (g->k == expr_kind)
    pretty_print_expr(g_expr_val(g));
  return g;
}

Geom* g_num(T a) { return new NumGeom(a); }
Geom* g_string(std::string s) { return new StringGeom(s); }
Geom* g_v2d(TV2 v) { return new V2dGeom(v); }
Geom* g_v3d(TV3 v) { return new V3dGeom(v); }
Geom* g_v3i(IV3 v) { return new V3iGeom(v); }
Geom* g_circle_arc(CircleArc v) { return new CircleArcGeom(v); }
Geom* g_mat(Matrix<T,4> mat)  { return new MatGeom(mat); }
Geom* g_array_v2d(Array<TV2> line) { return new ArrayV2dGeom(line); }
Geom* g_array_v2d(RawArray<TV2> line) { return g_array_v2d(line.copy()); }
Geom* g_array_v3d(Array<TV3> line) { return new ArrayV3dGeom(line); }
Geom* g_array_circle_arc(Array<CircleArc> arcs) { return new ArrayCircleArcGeom(arcs); }
Geom* g_array_circle_arc(RawArray<CircleArc> arcs) { return g_array_circle_arc(arcs.copy()); }
Geom* g_array_v3d(RawArray<TV3> line) { return g_array_v3d(line.copy()); }
Geom* g_array_v3i(Array<IV3> line) { return new ArrayV3iGeom(line); }
Geom* g_array_v3i(RawArray<IV3> line) { return g_array_v3i(line.copy()); }
Geom* g_nested_v2d(Nested<TV2> polyline) { return new NestedV2dGeom(polyline); }
Geom* g_nested_v3d(Nested<TV3> polyline) { return new NestedV3dGeom(polyline); }
Geom* g_nested_circle_arc(Nested<CircleArc> arcz) { return new NestedCircleArcGeom(arcz); }
Geom* g_poly(Nested<TV2> poly) { return new PolyGeom(poly); }
Geom* g_mesh(Mesh mesh) { return new MeshGeom(mesh); }

bool all_args_kind (std::vector<Geom*> args, int kind) {
  bool res = true;
  for (auto arg : args) 
    res = res && arg->k == kind;
  return res;
}

Geom* g_array_v3d (std::vector<Geom*> args) {
  Array< TV3 > points;
  for (auto arg : args)
    points.append(g_v3d_val(arg));
  return new ArrayV3dGeom(points);
}

Geom* g_array_v3i (std::vector<Geom*> args) {
  Array< IV3 > points;
  for (auto arg : args)
    points.append(g_v3i_val(arg));
  return new ArrayV3iGeom(points);
}

Geom* g_array_v2d (std::vector<Geom*> args) {
  Array< TV2 > points;
  for (auto arg : args) {
    points.append(g_v2d_val(arg));
  }
  return new ArrayV2dGeom(points);
}

Geom* g_nested_v3d (std::vector<Geom*> args) {
  Nested< TV3,false > lines;
  for (auto arg : args)
    lines.append(g_array_v3d_val(arg));
  lines.freeze();
  return new NestedV3dGeom(lines);
}

Geom* g_nested_v2d (std::vector<Geom*> args) {
  Nested< TV2,false > lines;
  for (auto arg : args)
    lines.append(g_array_v2d_val(arg));
  lines.freeze();
  return new NestedV2dGeom(lines);
}

Geom* g_poly (std::vector<Geom*> args) {
  Nested< TV2,false > lines;
  if (all_args_kind(args, array_v2d_kind)) {
    for (auto arg : args)
      lines.append(g_array_v2d_val(arg));
  } else if (all_args_kind(args, v2d_kind)) {
    Array< TV2 > points;
    for (auto arg : args)
      points.append(g_v2d_val(arg));
    lines.append(points);
  } else {
    error("Bad Poly Args"); return NULL;
  }
  lines.freeze();
  return new PolyGeom(lines);
}
Geom* g_arcs (std::vector<Geom*> args) {
  Nested< TV2,false > lines;
  if (all_args_kind(args, array_v2d_kind)) {
    for (auto arg : args)
      lines.append(g_array_v2d_val(arg));
  } else if (all_args_kind(args, v2d_kind)) {
    Array< TV2 > points;
    for (auto arg : args)
      points.append(g_v2d_val(arg));
    lines.append(points);
  } else {
    error("Bad Poly Args"); return NULL;
  }
  lines.freeze();
  return g_nested_circle_arc(lines);
}

Geom* g_pi(void) { return new NumGeom(M_PI); }
Geom* g_polygon_none(void) { return new PolyGeom(none_poly()); }
Geom* g_none2(void) { return g_polygon_none(); }
Geom* g_polygon_all(void) { return new PolyGeom(all_poly()); }
Geom* g_all2(void) { return g_polygon_all(); }
Geom* g_mesh_none(void) { return new MeshGeom(none_mesh()); }
Geom* g_none3(void) { return g_mesh_none(); }
Geom* g_mesh_all(void) { return new MeshGeom(all_mesh()); }
Geom* g_all3(void) { return g_mesh_all(); }
Geom* g_polygon_circle(float d) {
  return new PolyGeom(circle_poly(d, 16));
}
Geom* g_circle(Geom* a) {
  if (a->k == expr_kind)
    return expr_circle(g_expr_val(a));
  else 
    return g_polygon_circle(g_num_val(a));
}
Geom* g_polygon_square(float d) {
  return new PolyGeom(square_poly(d));
}
Geom* g_square(Geom* a) {
  if (a->k == expr_kind)
    return expr_square(g_expr_val(a));
  else 
    return g_polygon_square(g_num_val(a));
}
Geom* g_polygon_square_lo_hi(Geom* lo, Geom* hi) {
    return new PolyGeom(square_poly(g_v2d_val(lo), g_v2d_val(hi)));
}
Geom* g_square_lo_hi(Geom* lo, Geom* hi) {
  if (lo->k == expr_kind && hi->k == expr_kind)
    return expr_rect2(g_expr_val(lo), g_expr_val(hi));
  else 
    return g_polygon_square_lo_hi(lo, hi);
}
Geom* g_letter(Geom* a) {
  char c = g_string_val(a)[0];
  auto ol = stroke_char(c);
  return new NestedV2dGeom(ol);
}
Geom* g_text(Geom* a) {
  auto txt = g_string_val(a);
  auto res = stroke_text(txt);
  return new NestedV2dGeom(res);
}
Geom* g_elt(Geom* g, Geom* i) {
  if (g->k == poly_kind)
    return new ArrayV2dGeom(nested_elt(g_poly_val(g), (int)g_num_val(i)));
  else if (g->k == array_v2d_kind)
    return g_v2d(g_array_v2d_val(g)[(int)g_num_val(i)]);
  else if (g->k == array_v3d_kind)
    return g_v3d(g_array_v3d_val(g)[(int)g_num_val(i)]);
  else if (g->k == v3d_kind)
    return g_num(g_v3d_val(g)[(int)g_num_val(i)]);
  else if (g->k == v2d_kind)
    return g_num(g_v2d_val(g)[(int)g_num_val(i)]);
  else {
    error("Bad arg for elt"); return NULL;
  }
}
Geom* g_add(Geom* a, Geom* b) {
  if (a->k == num_kind && b->k == num_kind)
    return g_num(g_num_val(a) + g_num_val(b));
  else if (a->k == expr_kind && b->k == expr_kind)
    return expr_add(g_expr_val(a), g_expr_val(b));
  else if (a->k == v3d_kind && b->k == v3d_kind)
    return g_v3d(g_v3d_val(a) + g_v3d_val(b));
  else if (a->k == v2d_kind && b->k == v2d_kind)
    return g_v2d(g_v2d_val(a) + g_v2d_val(b));
  else {
    error("Bad args for add"); return NULL;
  }
}
Geom* g_sub(Geom* a, Geom* b) {
  if (a->k == num_kind && b->k == num_kind)
    return g_num(g_num_val(a) - g_num_val(b));
  else if (a->k == expr_kind && b->k == expr_kind)
    return expr_sub(g_expr_val(a), g_expr_val(b));
  else if (a->k == v3d_kind && b->k == v3d_kind)
    return g_v3d(g_v3d_val(a) - g_v3d_val(b));
  else if (a->k == v2d_kind && b->k == v2d_kind)
    return g_v2d(g_v2d_val(a) - g_v2d_val(b));
  else {
    error("Bad args for sub"); return NULL;
  }
}
Geom* g_dither(Geom* g) { 
  if (g->k == mesh_kind)
    return new MeshGeom(dither_mesh(g_mesh_val(g), 1e-16));
  else {
    error("Bad args for dither"); return NULL;
  }
}

Mesh g_mesh_val_copy (Geom* g) {
  return mesh_copy(g_mesh_val(g));
}

Geom* do_g_mul(Matrix<T,4> m, Geom* g) { 
  if (g->k == mesh_kind) {
    auto res = new MeshGeom(mesh_copy(mul(m, g_mesh_val_copy(g))));
    printf("%p = (%s * %p)\n", res, matrix_to_str(m).c_str(), g);
    printf("%p = %s\n", res, g_to_str_val(res).c_str());
    return res;
  } else if (g->k == poly_kind)
    return g_poly(mul_poly(m, g_poly_val(g)));
  else if (g->k == array_v2d_kind)
    return g_array_v2d(mul(m, g_array_v2d_val(g)));
  else if (g->k == array_v3d_kind)
    return g_array_v3d(mul(m, g_array_v3d_val(g)));
  else if (g->k == nested_v2d_kind)
    return g_nested_v2d(mul(m, g_nested_v2d_val(g)));
  else if (g->k == nested_v3d_kind)
    return g_nested_v3d(mul(m, g_nested_v3d_val(g)));
  else {
    error("Bad mul kind"); return NULL;
  }
}
Geom* g_polygon_mul(Geom* m, Geom* g) {
  return do_g_mul(g_mat_val(m), g);
}
Geom* g_mesh_mul(Geom* m, Geom* g) {
  return do_g_mul(g_mat_val(m), g);
}
Geom* g_mul(Geom* a, Geom* b) { 
  if (a->k == mat_kind)
    return do_g_mul(g_mat_val(a), b);
  else if (a->k == expr_kind && b->k == expr_kind)
    return expr_mul(g_expr_val(a), g_expr_val(b));
  else if (a->k == num_kind && b->k == num_kind)
    return g_num(g_num_val(a) * g_num_val(b));
  else if (a->k == num_kind && b->k == v3d_kind)
    return g_v3d(g_num_val(a) * g_v3d_val(b));
  else if (a->k == num_kind && b->k == v2d_kind)
    return g_v2d(g_num_val(a) * g_v2d_val(b));
  else {
    error("Bad args for mul"); return NULL;
  }
}

Geom* g_normalize(Geom* a) {
  if (a->k == v3d_kind)
    return g_v3d(g_v3d_val(a).normalized());
  else if (a->k == v2d_kind)
    return g_v2d(g_v2d_val(a).normalized());
  else {
    error("Bad args for normalize"); return NULL;
  }
  
}

Geom* g_magnitude(Geom* a) {
  if (a->k == v3d_kind)
    return g_num(g_v3d_val(a).magnitude());
  else if (a->k == v2d_kind)
    return g_num(g_v2d_val(a).magnitude());
  else {
    error("Bad args for magnitude"); return NULL;
  }
  
}
Geom* g_cross(Geom* a, Geom* b) {
  if (a->k == v3d_kind && b->k == v3d_kind) {
    return g_v3d(cross(g_v3d_val(a), g_v3d_val(b)));
  } else {
    error("Bad args for cross"); return NULL;
  }
}
Geom* g_dot(Geom* a, Geom* b) {
  if (a->k == v3d_kind && b->k == v3d_kind)
    return g_num(dot(g_v3d_val(a), g_v3d_val(b)));
  else if (a->k == num_kind && b->k == v2d_kind)
    return g_num(dot(g_v2d_val(a), g_v2d_val(b)));
  else {
    error("Bad args for dot"); return NULL;
  }
}

Geom* g_div(Geom* a, Geom* b) { 
  if (a->k == num_kind && b->k == num_kind)
    return g_num(g_num_val(a) * g_num_val(b));
  else if (a->k == expr_kind && b->k == expr_kind)
    return expr_div(g_expr_val(a), g_expr_val(b));
  else if (a->k == v3d_kind && b->k == num_kind)
    return g_v3d(g_v3d_val(a) / g_num_val(b));
  else if (a->k == v2d_kind && b->k == num_kind)
    return g_v2d(g_v2d_val(a) * g_num_val(b));
  else {
    error("Bad args for div"); return NULL;
  }
}
Geom* g_mag(Geom* v, Geom* g) { 
  return do_g_mul(scale_matrix(g_v3d_val(v)), g);
}
Geom* g_mag1(Geom* a, Geom* g) { 
  if (a->k == expr_kind && g->k == expr_kind)
    return expr_mag1(g_expr_val(a), g_expr_val(g));
  else
    return do_g_mul(scale_matrix(vec(g_num_val(a), g_num_val(a), g_num_val(a))), g);
}
Geom* g_xmag(Geom* a, Geom* g) { 
  if (a->k == expr_kind && g->k == expr_kind)
    return expr_xmag(g_expr_val(a), g_expr_val(g));
  else
    return do_g_mul(scale_matrix(vec(g_num_val(a), 1.0, 1.0)), g);
}
Geom* g_ymag(Geom* a, Geom* g) { 
  if (a->k == expr_kind && g->k == expr_kind)
    return expr_ymag(g_expr_val(a), g_expr_val(g));
  else
    return do_g_mul(scale_matrix(vec(1.0, g_num_val(a), 1.0)), g);
}
Geom* g_zmag(Geom* a, Geom* g) { 
  if (a->k == expr_kind && g->k == expr_kind)
    return expr_zmag(g_expr_val(a), g_expr_val(g));
  else
    return do_g_mul(scale_matrix(vec(1.0, 1.0, g_num_val(a))), g);
}
Geom* g_mov(Geom* v, Geom* g) {
  return do_g_mul(translation_matrix(g_v3d_val(v)), g);
}
Geom* g_xmov(Geom* a, Geom* g) {
  if (a->k == expr_kind && g->k == expr_kind)
    return expr_xmov(g_expr_val(a), g_expr_val(g));
  else
    return do_g_mul(translation_matrix(vec(g_num_val(a), 0.0, 0.0)), g);
}
Geom* g_ymov(Geom* a, Geom* g) {
  if (a->k == expr_kind && g->k == expr_kind)
    return expr_ymov(g_expr_val(a), g_expr_val(g));
  else
    return do_g_mul(translation_matrix(vec(0.0, g_num_val(a), 0.0)), g);
}
Geom* g_zmov(Geom* a, Geom* g) {
  if (a->k == expr_kind && g->k == expr_kind)
    return expr_zmov(g_expr_val(a), g_expr_val(g));
  else
    return do_g_mul(translation_matrix(vec(0.0, 0.0, g_num_val(a))), g);
}
Geom* g_rot(Geom* v, Geom* g) {
  TV3 rv = g_v3d_val(v);
  TV3 av = vec(degrees_to_radians(rv.x), degrees_to_radians(rv.y), degrees_to_radians(rv.z));
  return do_g_mul(rotation_matrix(av), g);
}
Geom* g_rot_from_to(Geom* from, Geom* to, Geom* g) {
  return do_g_mul(rotation_matrix(g_v3d_val(from), g_v3d_val(to)), g);
}
Geom* g_xrot(Geom* d, Geom* g) {
  if (d->k == expr_kind && g->k == expr_kind)
    return expr_xrot(g_expr_val(d), g_expr_val(g));
  else {
    T a = degrees_to_radians(g_num_val(d));
    T c=cos(a),s=sin(a);
    return do_g_mul(Matrix<T,4>(1,0,0,0,0,c,s,0,0,-s,c,0,0,0,0,1), g);
  }
}
Geom* g_yrot(Geom* d, Geom* g) {
  if (d->k == expr_kind && g->k == expr_kind)
    return expr_yrot(g_expr_val(d), g_expr_val(g));
  else {
    T a = degrees_to_radians(g_num_val(d));
    T c=cos(a),s=sin(a);
    return do_g_mul(Matrix<T,4>(c,0,-s,0,0,1,0,0,s,0,c,0,0,0,0,1), g);
  }
}
Geom* g_zrot(Geom* d, Geom* g) {
  if (d->k == expr_kind && g->k == expr_kind)
    return expr_zrot(g_expr_val(d), g_expr_val(g));
  else {
    T a = degrees_to_radians(g_num_val(d));
    T c=cos(a),s=sin(a);
    return do_g_mul(Matrix<T,4>(c,s,0,0,-s,c,0,0,0,0,1,0,0,0,0,1), g);
  }
}
Geom* g_reflect_x(Geom* g) {
  if (g->k == expr_kind)
    return expr_reflect_x(g_expr_val(g));
  else 
    return do_g_mul(Matrix<T,4>(-1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1), g);
}
Geom* g_reflect_y(Geom* g) { 
  if (g->k == expr_kind)
    return expr_reflect_y(g_expr_val(g));
  else 
    return do_g_mul(Matrix<T,4>(1,0,0,0, 0,-1,0,0, 0,0,1,0, 0,0,0,1), g);
}
Geom* g_reflect_z(Geom* g) {
  if (g->k == expr_kind)
    return expr_reflect_z(g_expr_val(g));
  else 
    return do_g_mul(Matrix<T,4>(1,0,0,0, 0,1,0,0, 0,0,-1,0, 0,0,0,1), g);
}
Geom* g_reflect_xy(Geom* g) {
  if (g->k == expr_kind)
    return expr_reflect_xy(g_expr_val(g));
  else 
    return do_g_mul(Matrix<T,4>(0,1,0,0, 1,0,0,0, 0,0,1,0, 0,0,0,1), g);
}
Geom* g_reflect_yz(Geom* g) {
  if (g->k == expr_kind)
    return expr_reflect_yz(g_expr_val(g));
  else 
    return do_g_mul(Matrix<T,4>(1,0,0,0, 0,0,1,0, 0,1,0,0, 0,0,0,1), g);
}
Geom* g_reflect_xz(Geom* g) {
  if (g->k == expr_kind)
    return expr_reflect_xz(g_expr_val(g));
  else
    return do_g_mul(Matrix<T,4>(0,0,1,0, 0,1,0,0, 1,0,0,0, 0,0,0,1), g);
}
Geom* g_mesh_union(Geom* a, Geom* b) {
  return new MeshGeom(mesh_copy(union_add(g_mesh_val_copy(a), g_mesh_val_copy(b))));
}
Geom* g_polygon_union(Geom* a, Geom* b) {
  return g_poly(union_add(g_poly_val(a), g_poly_val(b)));
}
Geom* g_union(Geom* a, Geom* b) {
  if (a->k == mesh_kind && b->k == mesh_kind)
    return g_mesh_union(a, b);
  else if (a->k == expr_kind && b->k == expr_kind)
    return expr_or(g_expr_val(a), g_expr_val(b));
  else if (is_poly(a) && is_poly(b))
    return g_polygon_union(a, b);
  else if (a->k == nested_circle_arc_kind && b->k == nested_circle_arc_kind)
    return g_nested_circle_arc_union(a, b);
  else {
    error("Bad args for union"); return NULL;
  }
}
Geom* g_mesh_intersection(Geom* a, Geom* b) {
  auto res = new MeshGeom(mesh_copy(intersection(g_mesh_val_copy(a), g_mesh_val_copy(b))));
  printf("%p = (%p & %p)\n", res, a, b);
  printf("%p = %s\n", a, g_to_str_val(a).c_str());
  printf("%p = %s\n", b, g_to_str_val(b).c_str());
  printf("%p = %s\n", res, g_to_str_val(res).c_str());
  return res;
}
Geom* g_polygon_intersection(Geom* a, Geom* b) {
  return g_poly(intersection(g_poly_val(a), g_poly_val(b)));
}
Geom* g_nested_circle_arc_find_overlapping_offsets(float d, Geom* a) {
  return new NestedCircleArcGeom(find_overlapping_offsets(g_nested_circle_arc_val(a), d));
}
Geom* g_nested_circle_arc_intersection(Geom* a, Geom* b) {
  return new NestedCircleArcGeom(circle_arc_intersection(g_nested_circle_arc_val(a), g_nested_circle_arc_val(b)));
}
Geom* g_nested_circle_arc_intersection1(Geom* a) {
  return new NestedCircleArcGeom(circle_arc_intersection(g_nested_circle_arc_val(a)));
}
static Nested<CircleArc> circle_arc_concat(Nested<CircleArc> a0, Nested<CircleArc> a1) {
  Nested<CircleArc,false> res;
  for (auto e0 : a0)
    res.append(e0);
  for (auto e1 : a1)
    res.append(e1);
  res.freeze();
  return res;
}
Geom* g_nested_circle_arc_concat(Geom* a, Geom* b) {
  return new NestedCircleArcGeom(circle_arc_concat(g_nested_circle_arc_val(a), g_nested_circle_arc_val(b)));
}
static Nested<CircleArc> nested_circle_arc_not (Nested<CircleArc> ca) {
  Nested<CircleArc> res = ca.copy();
  reverse_arcs(res);
  return res;
}
Geom* g_nested_circle_arc_difference(Geom* a, Geom* b) {
  return new NestedCircleArcGeom(circle_arc_union(g_nested_circle_arc_val(a), nested_circle_arc_not(g_nested_circle_arc_val(b))));
}
Geom* g_nested_circle_arc_not(Geom* a) {
  return new NestedCircleArcGeom(nested_circle_arc_not(g_nested_circle_arc_val(a)));
}
Geom* g_nested_circle_arc_union(Geom* a, Geom* b) {
  return new NestedCircleArcGeom(circle_arc_union(g_nested_circle_arc_val(a), g_nested_circle_arc_val(b)));
}
Geom* g_nested_circle_arc_open_offset(float r, Geom* g) {
  return new NestedCircleArcGeom(offset_open_arcs(g_nested_circle_arc_val(g), r));
}
Geom* g_nested_circle_arc_closed_offset(float r, Geom* g) {
  return new NestedCircleArcGeom(offset_arcs(g_nested_circle_arc_val(g), r));
}
Geom* g_nested_circle_arc_discretize(float max_deviation, int is_closed, Geom* g) {
  return new NestedV2dGeom(discretize_nested_arcs(g_nested_circle_arc_val(g), is_closed, max_deviation));
}
Geom* g_intersection(Geom* a, Geom* b) { 
  if (a->k == mesh_kind && b->k == mesh_kind)
    return g_mesh_intersection(a, b);
  else if (a->k == expr_kind && b->k == expr_kind)
    return expr_and(g_expr_val(a), g_expr_val(b));
  else if (is_poly(a) && is_poly(b))
    return g_polygon_intersection(a, b);
  else if (a->k == nested_circle_arc_kind && b->k == nested_circle_arc_kind)
    return g_nested_circle_arc_intersection(a, b);
  else {
    error("Bad args for intersection"); return NULL;
  }
}
Geom* g_mesh_difference(Geom* a, Geom* b) {
  auto res = new MeshGeom(mesh_copy(difference(g_mesh_val_copy(a), g_mesh_val_copy(b))));
  printf("%p = (%p \\ %p)\n", res, a, b);
  printf("%p = %s\n", a, g_to_str_val(a).c_str());
  // g_mesh_save(g_string_fab("a.obj"), a);
  printf("%p = %s\n", b, g_to_str_val(b).c_str());
  // g_mesh_save(g_string_fab("b.obj"), b);
  printf("%p = %s\n", res, g_to_str_val(res).c_str());
  // g_mesh_save(g_string_fab("r.stl"), res);
  return res;
}
Geom* g_polygon_difference(Geom* a, Geom* b) {
  return g_poly(difference(g_poly_val(a), g_poly_val(b)));
}
Geom* g_difference(Geom* a, Geom* b) {
  if (a->k == mesh_kind && b->k == mesh_kind)
    return g_mesh_difference(a, b);
  else if (a->k == expr_kind && b->k == expr_kind)
    return expr_rem(g_expr_val(a), g_expr_val(b));
  else if (is_poly(a) && is_poly(b))
    return g_polygon_difference(a, b);
  else if (a->k == nested_circle_arc_kind && b->k == nested_circle_arc_kind)
    return g_nested_circle_arc_difference(a, b);
  else {
    error("Bad args for difference"); return NULL;
  }
}
Geom* g_mesh_not(Geom* a) {
  return new MeshGeom(mesh_copy(invert_mesh(g_mesh_val_copy(a))));
}
Geom* g_polygon_not(Geom* a) {
  return g_poly(invert_poly(g_poly_val(a)));
}
Geom* g_not(Geom* a) {
  if (a->k == mesh_kind)
    return g_mesh_not(a);
  else if (a->k == expr_kind)
    return expr_not(g_expr_val(a));
  else if (is_poly(a))
    return g_polygon_not(a);
  else if (a->k == nested_circle_arc_kind)
    return g_nested_circle_arc_not(a);
  else {
    error("Bad args for not"); return NULL;
  }
}
Geom* g_mesh_offset(float r, Geom* g) {
  // return new MeshGeom(offset_mesh(1, g_num_val(a), g_mesh_val(g)));
  return new MeshGeom(mesh_copy(offset_mesh(r, g_mesh_val_copy(g))));
}
Geom* g_mesh_offset_rough(float r, Geom* g) {
  // return new MeshGeom(offset_mesh(1, g_num_val(a), g_mesh_val(g)));
  return new MeshGeom(mesh_copy(offset_mesh_rough(r, g_mesh_val_copy(g))));
}
Geom* g_polygon_offset(float r, Geom* g) {
  return g_poly(offset_poly(16, r, g_poly_val(g)));
}
Geom* g_polyline2_offset(float r, Geom* g) {
  return g_poly(offset_polyline(16, r, g_nested_v2d_val(g)));
}
Geom* g_open_offset(Geom* a, Geom* g) {
  if (g->k == nested_circle_arc_kind) {
    return g_nested_circle_arc_open_offset(g_num_val(a), g);
  } else {
    error("Bad args for offset"); return NULL;
  }
}
Geom* g_offset(Geom* a, Geom* g) {
  if (g->k == mesh_kind) {
    return g_mesh_offset(g_num_val(a), g);
  } else if (a->k == expr_kind && g->k == expr_kind) {
    return expr_sub(g_expr_val(g), g_expr_val(a));
  } else if (g->k == nested_v2d_kind) {
    return g_polyline2_offset(g_num_val(a), g);
  } else if (g->k == nested_circle_arc_kind) {
    return g_nested_circle_arc_closed_offset(g_num_val(a), g);
  } else if (is_poly(g)) {
    return g_polygon_offset(g_num_val(a), g);
  } else {
    error("Bad args for offset"); return NULL;
  }
}
Geom* g_hollow(Geom* a, Geom* m) { return g_difference(m, g_offset(g_num(-g_num_val(a)), m)); }
Geom* g_simplify(Geom* g) { return new MeshGeom(simplify_mesh(g_mesh_val(g))); }
Geom* g_cleanup(Geom* g) { return new MeshGeom(cleanup_mesh(g_mesh_val(g))); }
extern flo_t get_radius(void), get_threshold(void);
Geom* g_mesh_slice(float z, Geom* g) {
  return new PolyGeom(slice(z, g_mesh_val(g)));
}
Geom* g_slice(Geom* a, Geom* g) {
  if (a->k == num_kind && g->k == octree_kind) {
    return new PolyGeom(tree_slice(g_octree_val(g), g_num_val(a)));
  } else if (a->k == num_kind && g->k == expr_kind) {
    return new PolyGeom(tree_slice(expr_to_tree(g_expr_val(g), get_radius(), get_threshold()), g_num_val(a)));
  } else 
    return g_mesh_slice(g_num_val(a), g);
}
Geom* g_mesh_extrude(float h, Geom* p) {
  return new MeshGeom(extrude(h, g_poly_val(p)));
}
Geom* g_extrude(Geom* a, Geom* p) {
  if (a->k == expr_kind && p->k == expr_kind)
    return expr_extrude(g_expr_val(a), g_expr_val(p));
  else 
    return g_mesh_extrude(g_num_val(a), p);
}
Geom* g_polygon_thicken(float d, Geom* l) {
  return new PolyGeom(thicken(1, d, g_nested_v2d_val(l)));
}
Geom* g_mesh_thicken(float d, Geom* l) {
  return new MeshGeom(thicken(1, d, g_nested_v3d_val(l)));
}
Geom* g_thicken(Geom* a, Geom* l) {
  if (is_nested_v2d(l))
    return g_polygon_thicken(g_num_val(a), l);
  else //  (is_nested_v3d(l))
    return g_mesh_thicken(g_num_val(a), l);
}

Geom* g_mesh_sphere(float d) {
  return new MeshGeom(sphere_mesh(1, vec(0.0, 0.0, 0.0), 0.5 * d));
}
Geom* g_sphere(Geom* a) {
  if (a->k == expr_kind)
    return expr_sphere(g_expr_val(a));
  else
    return g_mesh_sphere(g_num_val(a));
}
Geom* g_mesh_cube(float d) {
  auto r = 0.5 * d; return new MeshGeom(cube_mesh(vec(-r, -r, -r), vec(r, r, r)));
}
Geom* g_cube(Geom* a) {
  if (a->k == expr_kind)
    return expr_cube(g_expr_val(a));
  else
    return g_mesh_cube(g_num_val(a));
}
Geom* g_mesh_cube_lo_hi(Geom* lo, Geom* hi) {
  auto res = new MeshGeom(cube_mesh(g_v3d_val(lo), g_v3d_val(hi)));
  printf("%p = cube(%s, %s)\n", res, g_to_str_val(lo).c_str(), g_to_str_val(hi).c_str());
  printf("%p = %s\n", res, g_to_str_val(res).c_str());
  return res;
}
Geom* g_cube_lo_hi(Geom* lo, Geom* hi) {
  return g_mesh_cube_lo_hi(lo, hi);
}
Geom* g_mesh_cone(float d, Geom* p) {
  return new MeshGeom(cone_mesh(d, g_poly_val(p)));
}
Geom* g_cone(Geom* a, Geom* p) {
  if (a->k == expr_kind && p->k == expr_kind)
    return expr_cone(g_expr_val(a), g_expr_val(p));
  else 
    return g_mesh_cone(g_num_val(a), p);
}
Geom* g_polygon_revolve(Geom* p) {
  return new MeshGeom(revolve(16, g_poly_val(p)));
}
Geom* g_revolve(Geom* p) {
  if (p->k == expr_kind)
    return expr_xrevolve(g_expr_val(p));
  else 
    return g_polygon_revolve(p);
}
Geom* g_mesh_hull(Geom* g) {
  return new MeshGeom(quick_hull_mesh(g_mesh_val(g)));
}
Geom* g_polygon_hull(Geom* g) {
  return new PolyGeom(quick_hull_poly(g_poly_val(g)));
}
Geom* g_hull(Geom* g) {
  if (g->k == mesh_kind)
    return g_mesh_hull(g);
  else if (g->k == poly_kind)
    return g_polygon_hull(g);
  else {
    error("Bad HULL type"); return NULL;
  }
}
// Geom* g_shear_x_z(Geom* z0, Geom* z1, Geom* dx0, Geom* dx1, Geom* m) {
//   return new MeshGeom(shear_x_z(g_num_val(z0), g_num_val(z1), g_num_val(dx0), g_num_val(dx1), g_mesh(m))); }

Geom* g_expr(Geom* g) {
  if (g->k == num_kind) {
    return expr_lit(g_num_val(g));
  } else if (g->k == poly_kind) {
    auto polygon = g_poly_val(g);
    return polygonize(polygon);
  } else if (g->k == mesh_kind) {
    auto mesh = g_mesh_val(g);
    return expr_mesh(mesh);
  } else {
    error("INCOMPATIBLE EXPR ARG");
    return NULL;
  } 
}

Geom* g_lit(flo_t a) { return expr_lit(a); }
Geom* g_x(void) { return expr_x(); }
Geom* g_y(void) { return expr_y(); }
Geom* g_z(void) { return expr_z(); }
Geom* g_neg(Geom* g) {
  if (g->k == num_kind)
    return g_num(-g_num_val(g));
  else 
    return expr_neg(g_expr_val(g));
}
Geom* g_min (Geom* a, Geom* b) {
  if (a->k == num_kind && b->k == num_kind)
    return g_num(min(g_num_val(a), g_num_val(b)));
  else 
    return expr_min(g_expr_val(a), g_expr_val(b));
}
Geom* g_max (Geom* a, Geom* b) {
  if (a->k == num_kind && b->k == num_kind)
    return g_num(max(g_num_val(a), g_num_val(b)));
  else 
    return expr_max(g_expr_val(a), g_expr_val(b));
}
Geom* g_pow(Geom* a, Geom* b) {
  if (a->k == num_kind && b->k == num_kind)
    return g_num(pow(g_num_val(a), g_num_val(b)));
  else 
    return expr_pow(g_expr_val(a), g_expr_val(b));
}
Geom* g_sqr(Geom* a) {
  if (a->k == num_kind)
    return g_num(g_num_val(a) * g_num_val(a));
  else 
    return expr_sqr(g_expr_val(a));
}
Geom* g_abs(Geom* a) {
  if (a->k == num_kind)
    return g_num(abs(g_num_val(a)));
  else 
    return expr_abs(g_expr_val(a));
}
Geom* g_sqrt(Geom* a) {
  if (a->k == num_kind)
    return g_num(sqrt(g_num_val(a)));
  else 
    return expr_sqrt(g_expr_val(a));
}
Geom* g_sin(Geom* g) {
  if (g->k == num_kind)
    return g_num(sin(g_num_val(g)));
  else 
    return expr_sin(g_expr_val(g));
}
Geom* g_cos(Geom* g) {
  if (g->k == num_kind)
    return g_num(cos(g_num_val(g)));
  else 
    return expr_cos(g_expr_val(g));
}
Geom* g_tan(Geom* a) {
  if (a->k == num_kind)
    return g_num(tan(g_num_val(a)));
  else 
    return expr_tan(g_expr_val(a));
}
Geom* g_asin(Geom* g) {
  if (g->k == num_kind)
    return g_num(asin(g_num_val(g)));
  else 
    return expr_asin(g_expr_val(g));
}
Geom* g_acos(Geom* g) {
  if (g->k == num_kind)
    return g_num(acos(g_num_val(g)));
  else 
    return expr_acos(g_expr_val(g));
}
Geom* g_atan(Geom* a) {
  if (a->k == num_kind)
    return g_num(atan(g_num_val(a)));
  else 
    return expr_atan(g_expr_val(a));
}
Geom* g_xform(Geom* nx, Geom* ny, Geom* nz, Geom* g) {
  return expr_xform(g_expr_val(nx), g_expr_val(ny), g_expr_val(nz), g_expr_val(g)); }
Geom* g_half(Geom* nx, Geom* ny, Geom* nz, Geom* d) {
  return expr_half(g_expr_val(nx), g_expr_val(ny), g_expr_val(nz), g_expr_val(d)); }
Geom* g_pyramid(Geom* xyd, Geom* zd) {
  return expr_pyramid(g_expr_val(xyd), g_expr_val(zd)); }
Geom* g_blend(Geom* a, Geom* b, Geom* c) { return expr_blend(g_expr_val(a), g_expr_val(b), g_expr_val(c)); }
Geom* g_shear(Geom* a, Geom* b, Geom* c, Geom* d, Geom* e) {
  return expr_shear_x_z(g_expr_val(a), g_expr_val(b), g_expr_val(c), g_expr_val(d), g_expr_val(e)); }
Geom* g_taper(Geom* dz, Geom* s0, Geom* s1, Geom* g) {
  // TODO: RECONCILE TWO
  if (g->k == expr_kind)
    return expr_taper(g_expr_val(dz), g_expr_val(s0), g_expr_val(s1), g_expr_val(g)); 
  else
    return new MeshGeom(taper_mesh(g_num_val(dz), g_num_val(s0), g_num_val(s1), g_poly_val(g)));
}
Geom* g_edge(Geom* x0, Geom* y0, Geom* x1, Geom* y1) {
  return expr_edge(g_expr_val(x0), g_expr_val(y0), g_expr_val(x1), g_expr_val(y1)); }
Geom* g_triangle(Geom* x0, Geom* y0, Geom* x1, Geom* y1, Geom* x2, Geom* y2) {
  return expr_triangle(g_expr_val(x0), g_expr_val(y0), g_expr_val(x1), g_expr_val(y1), g_expr_val(x2), g_expr_val(y2)); }
Geom* g_cylinder(Geom* xyd, Geom* zd) { return expr_cylinder(g_expr_val(xyd), g_expr_val(zd)); }
Geom* g_capsule(Geom* xyd, Geom* zd) { return expr_capsule(g_expr_val(xyd), g_expr_val(zd)); }

Geom* g_space(Geom* d) { return expr_space(g_expr_val(d)); }
Geom* g_xbox(Geom* a, Geom* b) { return expr_xbox(g_expr_val(a), g_expr_val(b)); }
Geom* g_ybox(Geom* a, Geom* b) { return expr_ybox(g_expr_val(a), g_expr_val(b)); }
Geom* g_zbox(Geom* a, Geom* b) { return expr_zbox(g_expr_val(a), g_expr_val(b)); }
Geom* g_align_xmin(Geom* a, Geom* b) { return expr_align_xmin(g_expr_val(a), g_expr_val(b)); }
Geom* g_align_ymin(Geom* a, Geom* b) { return expr_align_ymin(g_expr_val(a), g_expr_val(b)); }
Geom* g_align_zmin(Geom* a, Geom* b) { return expr_align_zmin(g_expr_val(a), g_expr_val(b)); }
Geom* g_align_xmax(Geom* a, Geom* b) { return expr_align_xmax(g_expr_val(a), g_expr_val(b)); }
Geom* g_align_ymax(Geom* a, Geom* b) { return expr_align_ymax(g_expr_val(a), g_expr_val(b)); }
Geom* g_align_zmax(Geom* a, Geom* b) { return expr_align_zmax(g_expr_val(a), g_expr_val(b)); }

Geom* g_to_tree(Geom* g, Geom* rad, Geom* thresh) {
  if (g->k == expr_kind) {
    return (Geom*)(expr_to_tree(g_expr_val(g), g_num_val(rad), g_num_val(thresh)));
  } else {
    error("INCOMPATIBLE TREE ARG");
    return NULL;
  }
}

Geom* g_to_mesh(Geom* g) {
  if (g->k == octree_kind) {
    return new MeshGeom(octree_to_mesh(g_octree_val(g)));
  } else if (g->k == expr_kind) {
    return new MeshGeom(octree_to_mesh(expr_to_tree(g_expr_val(g), get_radius(), get_threshold())));
  } else {
    error("INCOMPATIBLE MESH ARG");
    return NULL;
  } 
}

Expr* g_expr_val(Geom* g) {
  ensure(g->k == expr_kind, "EXPECTING EXPR KIND");
  return ((Expr*)g);
}


