/*
 * Project: FullereneViewer
 * Version: 1.0
 * Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
 */ 

#ifndef __CARBONALLOTROPE_H__
#define __CARBONALLOTROPE_H__

#include <stdio.h>
#include "Interactives.h"
#include "List.h"
#include "Ring.h"
#include "Carbon.h"
#include "Bond.h"
#include "ConnectedBoundary.h"
#include "SymmetryAxis.h"
#include "MyString.h"

class Representations;
class Pattern;
class BoundaryCarbons;

class CarbonAllotrope : public Interactives {
  // friend classes & functions

  // members
public:
  static bool s_need_representations;
  static bool s_need_representations_reflection;
  static bool s_need_all_axes;
  static bool s_need_major_axes;
  static bool s_need_principal_component_axes;

private:
  List<Ring> p_rings;
  List<Carbon> p_carbons;
  int p_scrap_no;
  int p_scrap_order;
  List<Carbon> p_centers;
  List<Bond> p_bonds;
  List<ConnectedBoundary> p_boundaries;
  bool p_has_reflection_symmetricity;
  List<SymmetryAxis> p_axes;
  Vector3 p_center_location;
  double p_Eigenvalue1, p_Eigenvalue2, p_Eigenvalue3;
  Vector3 p_Eigenvector1, p_Eigenvector2, p_Eigenvector3;

public:
  int ring_next_sequence;
  int carbon_next_sequence;
  double carbon_radius;
  int carbon_color;
  int bond_next_sequence;
  double bond_radius;
  int bond_color;
  int boundary_next_sequence;
  bool print_out_sequence_no;

  // private tools
private:
  void p_make_n_polygon(int n_members);
  void p_set_center();
  void p_list_most_inside_carbons_on_boundary(List<Carbon>& boundary);

  // constructors & the destructor
public:
  CarbonAllotrope();
  CarbonAllotrope* copy() const;
  virtual ~CarbonAllotrope();
  CarbonAllotrope& operator = (const CarbonAllotrope& you); /* dont use */
  void clean();

  // general purpose constructions
private:
  ErrorCode p_fill_hexagons_around(Ring* ring);
public:
  ErrorCode fill_hexagons_around_n_polygons(int n_members);
  ErrorCode append_n_polygon_at_carbon(int n_members, int carbon_sequence_no);
  ErrorCode append_n_polygon_at_carbons(int n_members, const int* carbon_sequence_nos);
  ErrorCode append_n_polygon_at_bond(int n_members, int bond_sequence_no);

  // fullerene constructions
private:
  int p_calculate_period(const List<Carbon>& boundary, int& offset);
public:
  ErrorCode enlarge_cylinder_by_n_polygons(Pattern* n_pattern, int& result_number);
  ErrorCode make_fullerene(int distance);
  ErrorCode
  fill_n_polygons_around_carbons_closed_to_center_and_pentagons(int n_members,
                                                                int& number_of_results);
  ErrorCode fill_n_polygon_around_carbon(int n_members, Carbon* carbon,
                                         BoundaryCarbons& boundary);
  ErrorCode make_symmetric_scrap(int scrap_no);

  // carbon nano-tube constructions
private:
  char* p_determine_permutation_of_lattice_basis(int n, int m);
  void p_make_equator_by_chiral_characteristic(int n, int m);
public:
  void make_equator_by_chiral_characteristic(int n, int m, int h);
  void make_y_branch();
  void close_force();
  void close_normally_once();

  // construction rollback
  void rollback(int ring_next_sequence, int carbon_next_sequence, int bond_next_sequence);

  // distance
private:
  void p_calculate_distances_to_pentagons();
  int p_maximum_distance_to_set();
public:
  void calculate_distances_to_set(List<Carbon>& set);
  void print_distances_between_pentagons();

  // determining normal vector
public:
  void set_clockwise(int clockwise);
  void reset_done();

  // interactions
protected:
  virtual void p_reset_interaction();
public:
  virtual const Vector3& get_center_location() const { return p_center_location; }

public:
  void register_interactions();

  // representation
private:
  void p_all_representations_half(Representations* results, int clockwise);
public:
  void all_representations(Representations* results);

  // six-views drawing
public:
  void reset_three_axes();
  void calculate_three_axes();
  void get_primary_Eigenvalue_and_Eigenvector(double& Eigenvalue,
                                              Vector3& Eigenvector) const
  { Eigenvalue = p_Eigenvalue1; Eigenvector = p_Eigenvector1; }
  void get_secondary_Eigenvalue_and_Eigenvector(double& Eigenvalue,
                                                Vector3& Eigenvector) const
  { Eigenvalue = p_Eigenvalue2; Eigenvector = p_Eigenvector2; }
  void get_tertiary_Eigenvalue_and_Eigenvector(double& Eigenvalue,
                                               Vector3& Eigenvector) const
  { Eigenvalue = p_Eigenvalue3; Eigenvector = p_Eigenvector3; }
  Matrix3 generate_matrix_of_six_views();
  void get_range_of_six_views(const Matrix3& mosv,
                              Vector3& minimum, Vector3& maximum) const;

  // mathematics
public:
  int Euler_characteristic() const;
  void count_carbons(int& number_of_carbons_with_one_ring,
                     int& number_of_carbons_with_two_rings,
                     int& number_of_carbons_with_three_rings) const;

  // I/O
public:
  void print_detail();
  void print_boundary_representations();
  void print_axes() const;
  void print_axes_summary(FILE* fptr = stdout) const;
  void draw_by_POVRay(const MyString& file_name_base, double delta, int steps,
                      int divisions);
  void OpenGL_to_POVRay(const MyString& file_name_base, int view,
                        const Quaternion& rotation);
  void draw_six_views_by_POVRay(const MyString& file_name_base);
  static void execute_POVRay(const MyString& pov_name);
  void draw_force_to_circle_by_POVRay(const MyString& file_name_base,
                                      List<Carbon>& cutend_list,
                                      double delta, int steps, int divisions);
  void draw_force_to_circle_by_POVRay(const MyString& file_name_base,
                                      double delta, int steps, int divisions);
  void draw_development_view_by_POVRay(const MyString& file_name_base, Ring* cutend_ring,
                                       double delta, int steps, int divisions);
  void print_POVRay_scene_description(FILE* fptr) const;
  void print_POVRay_scene_description(FILE* fptr, const Matrix3& rot, const Vector3& move,
                                      bool clipped_by_Z_non_negative) const;
  void memory_shape(const MyString& file_name_base) const;
  void recall_shape(const MyString& file_name_base);

  // member accessing methods
public:

  // rings
  int number_of_rings() const { return p_rings.length(); }
  void register_ring(Ring* ring);
  void remove_ring(Ring* ring);
  Ring* get_ring(int index) const;
  Ring* get_ring_by_sequence_no(int sequence_no) const;

  // carbons
  int number_of_carbons() const { return p_carbons.length(); }
  void register_carbon(Carbon* ring);
  Carbon* get_carbon(int index) const;
  Carbon* get_carbon_by_sequence_no(int sequence_no) const;

  // bonds
  int number_of_bonds() const { return p_bonds.length(); }
  void register_bond(Bond* ring);
  Bond* get_bond(int index) const;
  Bond* get_bond_by_sequence_no(int sequence_no) const;

  // connected boundaries
  // For temporary use in construction phase.
  int list_oldest_connected_boundary(List<Carbon>& result) const;
  int list_oldest_connected_boundary_carbons(BoundaryCarbons& result) const;
  int list_oldest_connected_boundary(List<Carbon>& result,
                                     const List<Carbon>& ignores) const;
  int list_newborn_connected_boundary(List<Carbon>& result) const;
  // CarbonAllotrope memories all boundaries after construction done.
  void all_boundaries();
  int number_of_boundaries() const { return p_boundaries.length(); }
  void register_boundary(ConnectedBoundary* boundary);
  ConnectedBoundary* get_boundary(int index) const;
  ConnectedBoundary* get_boundary_by_sequence_no(int sequence_no) const;

  // reflections symmetricity
  void has_reflection_symmetricity(bool yes) { p_has_reflection_symmetricity = yes; }
  bool has_reflection_symmetricity() const { return p_has_reflection_symmetricity; }

  // axes
  int number_of_axes() const { return p_axes.length(); }
  SymmetryAxis* get_axis(int index) const;
  void get_major_axes(List<SymmetryAxis>& result) const;
  void register_axis(SymmetryAxis* axis);

};

#endif /* __CARBONALLOTROPE_H__ */

/* Local Variables:	*/
/* mode: c++		*/
/* End:			*/
