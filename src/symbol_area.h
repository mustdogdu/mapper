/*
 *    Copyright 2012, 2013 Thomas Schöps
 *
 *    This file is part of OpenOrienteering.
 *
 *    OpenOrienteering is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    OpenOrienteering is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with OpenOrienteering.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _OPENORIENTEERING_SYMBOL_AREA_H_
#define _OPENORIENTEERING_SYMBOL_AREA_H_

#include "symbol.h"
#include "symbol_properties_widget.h"

QT_BEGIN_NAMESPACE
class QCheckBox;
class QDoubleSpinBox;
class QLabel;
class QListWidget;
class QPushButton;
class QSpinBox;
class QToolButton;
class QXmlStreamReader;
class QXmlStreamWriter;
QT_END_NAMESPACE

class ColorDropDown;
class SymbolSettingDialog;
class PointSymbolEditorWidget;
class PathObject;
class PointObject;
class LineSymbol;
class PointSymbol;
class SymbolPropertiesWidget;
class SymbolSettingDialog;
class MapEditorController;

class AreaSymbol : public Symbol
{
friend class AreaSymbolSettings;
friend class PointSymbolEditorWidget;
friend class OCAD8FileImport;
public:
	struct FillPattern
	{
		enum Type
		{
			LinePattern = 1,
			PointPattern = 2
		};
		
		Type type;
		float angle;			// 0 to 2*M_PI
		bool rotatable;
		int line_spacing;		// as usual, in 0.001mm
		int line_offset;
		int offset_along_line;	// only if type == PointPattern
		
		const MapColor* line_color;	// only if type == LinePattern
		int line_width;			// line width if type == LinePattern
		
		int point_distance;		// point distance if type == PointPattern
		PointSymbol* point;		// contained point symbol if type == PointPattern
		
		QString name;			// a display name (transient)
		
		FillPattern();
		void save(QIODevice* file, Map* map);
		bool load(QIODevice* file, int version, Map* map);
		void save(QXmlStreamWriter& file, const Map& map) const;
		void load(QXmlStreamReader& xml, Map& map, SymbolDictionary& symbol_dict);
		bool equals(FillPattern& other, Qt::CaseSensitivity case_sensitivity);
		void createRenderables(QRectF extent, float delta_rotation, const MapCoord& pattern_origin, ObjectRenderables& output);
		void createLine(MapCoordVectorF& coords, float delta_offset, LineSymbol* line, PathObject* path, PointObject* point_object, ObjectRenderables& output);
		void scale(double factor);
	};
	
	AreaSymbol();
	virtual ~AreaSymbol();
	virtual Symbol* duplicate(const MapColorMap* color_map = NULL) const;
	
	virtual void createRenderables(Object* object, const MapCoordVector& flags, const MapCoordVectorF& coords, ObjectRenderables& output);
	void createRenderablesNormal(Object* object, const MapCoordVector& flags, const MapCoordVectorF& coords, ObjectRenderables& output);
	virtual void colorDeleted(const MapColor* color);
	virtual bool containsColor(const MapColor* color) const;
	virtual const MapColor* getDominantColorGuess() const;
	virtual void scale(double factor);
	
	// Getters / Setters
	inline const MapColor* getColor() const {return color;}
	inline void setColor(const MapColor* color) {this->color = color;}
	inline int getMinimumArea() const {return minimum_area; }
	inline int getNumFillPatterns() const {return (int)patterns.size();}
	inline void setNumFillPatterns(int count) {patterns.resize(count);}
	inline FillPattern& getFillPattern(int i) {return patterns[i];}
	inline const FillPattern& getFillPattern(int i) const {return patterns[i];}
	bool hasRotatableFillPattern() const;
	virtual SymbolPropertiesWidget* createPropertiesWidget(SymbolSettingDialog* dialog);
	
protected:
	virtual void saveImpl(QIODevice* file, Map* map);
	virtual bool loadImpl(QIODevice* file, int version, Map* map);
	virtual void saveImpl(QXmlStreamWriter& xml, const Map& map) const;
	virtual bool loadImpl(QXmlStreamReader& xml, Map& map, SymbolDictionary& symbol_dict);
	virtual bool equalsImpl(Symbol* other, Qt::CaseSensitivity case_sensitivity);
	
	const MapColor* color;
	int minimum_area;	// in mm^2 // FIXME: unit (factor) wrong
	std::vector<FillPattern> patterns;
};

class AreaSymbolSettings : public SymbolPropertiesWidget
{
Q_OBJECT
public:
	AreaSymbolSettings(AreaSymbol* symbol, SymbolSettingDialog* dialog);
	
	virtual void reset(Symbol* symbol);
	
	/**
	 * Updates the general area fields (not related to patterns)
	 */
	void updateAreaGeneral();
	
	void addPattern(AreaSymbol::FillPattern::Type type);
	
signals:
	void switchPatternEdits(int layer);
	
public slots:
	void selectPattern(int index);
	void addLinePattern();
	void addPointPattern();
	void deleteActivePattern();
	
protected:
	void clearPatterns();
	void loadPatterns();
	void updatePatternNames();
	void updatePatternWidgets();
	
protected slots:
	void colorChanged();
	void minimumSizeChanged(double value);
	void patternAngleChanged(double value);
	void patternRotatableClicked(bool checked);
	void patternSpacingChanged(double value);
	void patternLineOffsetChanged(double value);
	void patternOffsetAlongLineChanged(double value);
	void patternColorChanged();
	void patternLineWidthChanged(double value);
	void patternPointDistChanged(double value);
	
private:
	AreaSymbol* symbol;
	Map* map;
	MapEditorController* controller;
	std::vector<AreaSymbol::FillPattern>::iterator active_pattern;
	
	ColorDropDown*  color_edit;
	QDoubleSpinBox* minimum_size_edit;
	
	QListWidget*    pattern_list;
	QToolButton*    add_pattern_button;
	QPushButton*    del_pattern_button;
	
	QLabel*         pattern_name_edit;
	QDoubleSpinBox* pattern_angle_edit;
	QCheckBox*      pattern_rotatable_check;
	QDoubleSpinBox* pattern_spacing_edit;
	QDoubleSpinBox* pattern_line_offset_edit;
	QDoubleSpinBox* pattern_offset_along_line_edit;
	
	ColorDropDown*  pattern_color_edit;
	QDoubleSpinBox* pattern_linewidth_edit;
	QDoubleSpinBox* pattern_pointdist_edit;
};

#endif