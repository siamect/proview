#ifndef glow_growannot_h
#define glow_growannot_h

#include "glow_annot.h"
#include "glow_text.h"
#include "glow_rect.h"

/*! \file glow_growannot.h
    \brief Contains the GrowAnnot class. */
/*! \addtogroup Glow */
/*@{*/


//! Class for drawing and marking an annotation in a subgraph.
/*! A text object for drawing an annotation text with specified font, size and color.
  Detection of event when the object is clicked on, moved etc.
  The GrowAnnot object is the object displayed in the editor when the subgraph is edited.
  As a memeber of a NodeClass only the GlowAnnot part of the object appears. When the
  object is saved as a subgraps in a pwsg-file only the GlowAnnot part is saved.
*/
class GrowAnnot : public GlowAnnot {
 public:
  //! Constuctor
  /*!
    \param glow_ctx 	The glow context.
    \param x		x coordinate for position.
    \param y		y coordinate for position.
    \param annot_num	Annotation number.
    \param d_type 	Text drawtype.
    \param color_d_type Color drawtype.
    \param t_size	Text size.
    \param a_type	Type of annotation. One line or multiline.
    \param rel_pos	No used.
    \param display_lev	Level when the object is visible.
    \param nodraw	Don't draw the object now.
  */
  GrowAnnot( GlowCtx *glow_ctx, double x = 0, double y = 0,
	int annot_num = 0, glow_eDrawType d_type = glow_eDrawType_TextHelveticaBold,
	glow_eDrawType color_d_type = glow_eDrawType_Line,
	int t_size = 2, glow_eAnnotType a_type = glow_eAnnotType_OneLine,
	int rel_pos = 0, glow_mDisplayLevel display_lev = glow_mDisplayLevel_1,
	int nodraw =0):
	GlowAnnot(glow_ctx,x,y,annot_num,d_type,color_d_type,t_size,a_type,
	rel_pos,display_lev), temporary_scale(0) {};

  //! Save the content of the object to file.
  /*!
    \param fp	Ouput file.
    \param mode	Save as subgraph or graph.

    If the mode is glow_eSaveMode_Subgraph only the GlowAnnot part of the annoation is saved.
  */
  void save( ofstream& fp, glow_eSaveMode mode);


  //! Read the content of the object from file.
  /*!
    \param fp	Input file.
  */
  void open( ifstream& fp);

  //! Get the object type
  /*!
    \return The type of the object.
  */
  glow_eObjectType type() { return glow_eObjectType_GrowAnnot;};

  GlowTransform trf;		//!< Transformation matrix of object.
  int		temporary_scale; //!< Not used.

  //! Draw the object.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param highlight	Draw with highlight colors.
    \param hot		Draw as hot, with larger line width.
    \param node		Parent node. Can be zero.
    \param colornode	The node that controls the color of the object. Can be zero.

    The object is drawn with border, fill and shadow. If t is not zero, the current tranform is
    multiplied with the parentnodes transform, to give the appropriate coordinates for the drawing.
  */
  void draw( GlowTransform *t, int highlight, int hot, void *node, void *colornode);

  //! Erase the object.
  /*!
    \param t		Transform of parent node.
    \param hot		Draw as hot, with larger line width.
    \param node		Parent node. Can be zero.
  */
  void erase( GlowTransform *t, int hot, void *node);

  //! Redraw the background of the annotation
  /*!
    \param t		Transform of parent node.
    \param hot		Draw as hot, with larger line width.
    \param node		Parent node. Can be zero.
  */
  void erase_background( GlowTransform *t, int hot, void *node);

  //! Draw the object in the navigation window.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param highlight	Draw with highlight colors.
    \param node		Parent node. Can be zero.
    \param colornode	The node that controls the color of the object. Can be zero.
  */
  void nav_draw( GlowTransform *t, int highlight, void *node, void *colornode);

  //! Erase the object in the navigation window.
  /*!
    \param t		Transform of parent node.
    \param node		Parent node. Can be zero.
  */
  void nav_erase( GlowTransform *t, void *node);
  
  //! Calculate the border for a set of objects or for a parent node.
  /*!
    \param t		Transform.
    \param x_right	Right limit.
    \param x_left	Left limit.
    \param y_high	High limit.
    \param y_low	Low limit.
    
    If the borders of the objects exceeds a limit, the limit is adjusted to the
    border of the object.
  */
  void get_borders( GlowTransform *t, double *x_right, double *x_left, 
	double *y_high, double *y_low);

  //! Export the object as java code.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param node		Parent node. Can be zero.
    \param pass		Export pass.
    \param shape_cnt	Current index in a shape vector.
    \param node_cnt	Counter used for javabean name. Not used for this kind of object.
    \param in_nc	Member of a nodeclass. Not used for this kind of object.
    \param fp		Output file.

    The object is transformed to the current zoom factor, and GlowExportJBean is used to generate
    java code for the shape.
  */
  void export_javabean( GlowTransform *t, void *node,
	glow_eExportPass pass, int *shape_cnt, int node_cnt, int in_nc, ofstream &fp);

  //! Export the font as java code.
  /*!
    \param t		Transform of parent node. Can be zero.
    \param node		Parent node. Can be zero.
    \param pass		Export pass.
    \param fp		Output file.

    GlowExortJBean is used to generate code for the font.
  */
  void export_javabean_font( GlowTransform *t, void *node,
	glow_eExportPass pass, ofstream &fp);

  //! Conversion between different versions of Glow
  /*!
    \param version	Version to convert to.
  */
  void convert( glow_eConvert version);

  //! Get text size an color.
  /*!
    \param node		Node.
    \param t_size	Annotation text size.
    \param t_drawtype Annotation text drawtype.
    \param t_color	Annotation text color.
  */
  void get_annotation_info( void *node, int *t_size, glow_eDrawType *t_drawtype,
			    glow_eDrawType *t_color);
};

/*@}*/
#endif
