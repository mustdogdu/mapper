/*
 *    Copyright 2012, 2013 Thomas Schöps, Kai Pastor
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


#ifndef _OPENORIENTEERING_MAP_COLOR_H_
#define _OPENORIENTEERING_MAP_COLOR_H_

#include <vector>

#include <qmath.h>
#include <QColor>
#include <QString>
#include <QHash>

class Map;
class MapColor;

/**
 * The MapColorCmyk class provides a datatype for storing and transfering 
 * opaque CMYK colors. 
 * 
 * Components (c, m, y, k) are floats in the range [0.0; 1.0].
 */
struct MapColorCmyk
{
	/** The cyan component. */
	float c;
	/** The magenta component. */
	float m;
	/** The yellow component. */
	float y;
	/** The black component (aka key). */
	float k;
	
	/** Constructs a black color. */
	MapColorCmyk();
	
	/** Constructs a color with the given components. */
	MapColorCmyk(float c, float m, float y, float k);
	
	/** Constructs a copy of the given CMYK color. */
	MapColorCmyk(const MapColorCmyk& other);
	
	/** Constructs a CMYK color of the given QColor. Used for type conversions. */
	MapColorCmyk(const QColor& other);
	
	/** Assigns another color's value to this color. */
	void operator=(const MapColorCmyk& other);
	
	/** Converts this color to a QColor. */
	operator QColor() const;
	
	/** Returns true if this color is black. */
	bool isBlack() const;
	
	/** Returns true if this color is white. */
	bool isWhite() const;
};

/** Returns true iff the MapColorCmyk are equal in all components. */
bool operator==(const MapColorCmyk& lhs, const MapColorCmyk& rhs);

/** Returns true iff the MapColorCmyk differ in at least one components. */
bool operator!=(const MapColorCmyk& lhs, const MapColorCmyk& rhs);


/**
 * The MapColorRgb class provides a datatype for storing and transfering 
 * opaque RGB colors. 
 * 
 * Components (r, g, b) are floats in the range [0.0; 1.0].
 */
struct MapColorRgb
{
	/** The red component. */
	float r;
	/** The green component. */
	float g;
	/** The blue component. */
	float b;
	
	/** Constructs a black color. */
	MapColorRgb();
	
	/** Constructs a color with the given components. */
	MapColorRgb(float r, float g, float b);
	
	/** Constructs a copy of the given RGB color. */
	MapColorRgb(const MapColorRgb& other);
	
	/** Constructs a RGB color of the given QColor. Used for type conversions. */
	MapColorRgb(const QColor& other);
	
	/** Assigns another color's value to this color. */
	void operator=(const MapColorRgb& other);
	
	/** Converts this color to a QColor. */
	operator QColor() const;
	
	/** Returns true if this color is black. */
	bool isBlack() const;
	
	/** Returns true if this color is white. */
	bool isWhite() const;
};

/** Returns true if both MapColorRgb are equal in all components. */
bool operator==(const MapColorRgb& lhs, const MapColorRgb& rhs);

/** Returns true iff the MapColorRgb differ in at least one components. */
bool operator!=(const MapColorRgb& lhs, const MapColorRgb& rhs);


/**
 * The SpotColorComponent datatype describes the use of a spot color in a 
 * screen or overprint to create another color.
 */
struct SpotColorComponent
{
	/** A map color which is a spot color. */
	MapColor* spot_color;
	
	/** The factor describes the halftoning (screen).
	 *  It is a value in the range [0.0; 1.0]. */
	float factor;
	
	/** Constructs a component with an undefined spot color and halftoning. */
	SpotColorComponent();
	
	/** Constructs a component for the given spot color and halftoning. */
	SpotColorComponent(MapColor* spot_color, float factor);
	
	/** Returns true iff the spot color is defined. */
	bool isValid() const;
};


/**
 * The SpotColorComponents type is a STL container that stores all 
 * SpotColorComponent elements which make up a particular map color.
 */
typedef std::vector<SpotColorComponent> SpotColorComponents;


/**
 * The MapColor class provides colors which may be used by symbols (and 
 * objects). Apart from the mere color, it specifies how to output the color
 * to different type of devices and optionally how the color was composed 
 * from other colors.
 */
class MapColor
{
public:
	/** 
	 * SpecialProperties provides identifiers for (pseudo-)colors serving
	 * particular purposes in the program.
	 */
	enum SpecialPriorities
	{
		CoveringRed   = -1005,
		CoveringWhite = -1000,  // used for tool helper line colors	
		Undefined     = -500,
		Reserved      = -1      // used to mark renderables which should not be inserted into the map
	};
	
	/** 
	 * ColorMethod provides identifiers for methods on how to determine a 
	 * particular realization of a color.
	 */
	enum ColorMethod
	{
		UndefinedMethod = 0,
		CustomColor     = 1,
		SpotColor       = 2,
		CmykColor       = 4,
		RgbColor        = 8,
		Knockout        = 16
	};
	
	/** Constructs a black CMYK map color of undefined priority.*/
	MapColor();
	
	/** Constructs a black CMYK map color with the given priority. */
	MapColor(int priority);
	
	/** Constructs a black CMYK map color with the given name and priority. */
	MapColor(const QString& name, int priority);
	
	/** Returns a copy of the color. */
	MapColor* duplicate() const;
	
	
	/** Returns a QColor represenation (reference) of the CMYK values.
	 * 
	 * The returned const QColor& can be passed to QtGui operations. 
	 * MapColor maintains an own QColor instance for efficienc
	 */
	operator const QColor&() const;
	
	/** Converts the current CMYK values to a QRgb. */
	operator QRgb() const;
	
	
	/** Returns the color's name for the mapping context. */
	const QString& getName() const;
	
	/** Sets the color's name for the mapping context. */
	void setName(const QString& name);
	
	/** Returns the color's priority. */
	int getPriority() const;
	
	/** 
	 * Sets the color's priority.
	 * Normally you don't want to call this directly.
	 */
	void setPriority(int priority);
	
	/** @deprecated Returns the color's opacity. */
	float getOpacity() const;
	
	/** @deprecated Sets the color's opacity. */
	void setOpacity(float opacity);
	
	
	/** 
	 * Returns how the spot color is to be created.
	 * 
	 * Returns UndefinedMethod, SpotColor (for a full tone single color 
	 * referenced by name), or CustomColor (for a color created from named 
	 * colors using halftoning (screens) and overprint.
	 */
	ColorMethod getSpotColorMethod() const;
	
	/** 
	 * Returns the name for the single spot color or a label for the spot color 
	 * composition which realizes this map color.
	 * Returns an empty string for an UndefinedMethod.
	 */
	const QString& getSpotColorName() const;
	
	/**
	 * Sets the name of a single spot color which realizes this map color,
	 * and sets the spot color method to SpotColor.
	 */
	void setSpotColorName(const QString& spot_color_id);
	
	/** 
	 * Sets the given components (i.e. screens and/or overprint) for the color,
	 * and sets the spot color method to CustomColor.
	 */
	void setSpotColorComposition(const SpotColorComponents& components);
	
	/** 
	 * Returns the components of the spot color realization of this color.
	 * Returns an empty list if the spot color method is not CustomColor.
	 */
	const SpotColorComponents& getComponents() const;
	
	/**
	 * Sets the value of knockout flag for spot color printing.
	 */
	void setKnockout(bool flag);
	
	/**
	 * Returns the value of the knockout flag.
	 */
	bool getKnockout() const;
	
	
	/**
	 * Returns how the CMYK color value is determined.
	 * 
	 * Returns CustomColor (for custom CMYK values, e.g. for named spot colors),
	 * SpotColor (for values determined from evaluation the spot color composition),
	 * or RgbColor (for values directly derived from the current RGB values).
	 */
	ColorMethod getCmykColorMethod() const;
	
	/** Returns the map color's CMYK values. */
	const MapColorCmyk& getCmyk() const;
	
	/** Sets the CMYK values, and sets the CMYK color method to CustomColor. */
	void setCmyk(const MapColorCmyk& cmyk);
	
	/** 
	 * Determines the CMYK values from the spot color composition,
	 * and sets the CMYK color method to SpotColor.
	 * 
	 * The spot color method must be CustomColor.
	 * If the spot color composition is empty, the spot color method is 
	 * changed to CustomColor, and the CMYK color method is changed to 
	 * CustomColor.
	 */ 
	void setCmykFromSpotColors();
	
	/**
	 * Determines the CMYK from the current RGB value, 
	 * and sets the CMYK color method to RgbColor.
	 * 
	 * If the RGB color method is CmykColor, it is changed to CustomColor.
	 */
	void setCmykFromRgb();
	
	
	/**
	 * Returns how the RGB color value is determined.
	 * 
	 * Returns CustomColor (for custom RGB values),
	 * SpotColor (for values determined from evaluation the spot color composition),
	 * or CmykColor (for values directly derived from the current CMYK value).
	 */
	ColorMethod getRgbColorMethod() const;
	
	/** Returns the map color's RGB values. */
	const MapColorRgb& getRgb() const;
	
	/** Sets the RGB values, and sets the RGB color method to CustomColor. */
	void setRgb(const MapColorRgb& rgb);
	
	/** 
	 * Determines the RGB values from the spot color composition,
	 * and sets the RGB color method to SpotColor.
	 * 
	 * The spot color method must be CustomColor.
	 * If the spot color composition is empty, the spot color method is 
	 * changed to CustomColor, and the RGB color method is changed to 
	 * CustomColor.
	 */ 
	void setRgbFromSpotColors();
	
	/**
	 * Determines the RGB from the current CMYK value, 
	 * and sets the RGB color method to CmykColor.
	 * 
	 * If the CMYK color method is RgbColor, it is changed to CustomColor.
	 */
	void setRgbFromCmyk();
	
	
	/** Returns true if this color is black. */
	bool isBlack() const;
	
	/** Returns true if this color is white. */
	bool isWhite() const;
	
	
	/** Compares this color and another. */
	bool equals(const MapColor& other, bool compare_priority) const;
	
	/** Compares two colors given by pointers.
	 *  Returns true if the colors are equal or if both pointers are NULL. */
	static bool equal(const MapColor* color, const MapColor* other);
	
	/** Returns true if this color's priority is less than the other's. */
	bool comparePriority(const MapColor& other) const;
	
protected:
	/** 
	 * Returns a CMYK color determined from the cmyk color of the spot color
	 * components.
	 */
	MapColorCmyk cmykFromSpotColors() const;
	
	/** 
	 * Returns a RGB color determined from the cmyk color of the spot color
	 * components.
	 */
	MapColorRgb rgbFromSpotColors() const;
	
	
	QString name;
	int priority;
	
	MapColorCmyk cmyk;
	MapColorRgb rgb;
	float opacity;
	
	QColor q_color;
	
	char spot_color_method;
	char cmyk_color_method;
	char rgb_color_method;
	char flags;
	
	QString spot_color_name;
	SpotColorComponents components;
};

/** Returns true if both MapColor are equal in all components. */
bool operator==(const MapColor& lhs, const MapColor& rhs);

/** Returns true iff the MapColor differ in at least one components. */
bool operator!=(const MapColor& lhs, const MapColor& rhs);


/** MapColorMap provides a mapping from one map color to another. */
typedef QHash<const MapColor*, const MapColor*> MapColorMap;


// ### MapColorCmyk inline code ###

inline
MapColorCmyk::MapColorCmyk()
 : c(0.0f), m(0.0f), y(0.0f), k(1.0f)
{
	Q_ASSERT(isBlack());
}

inline
MapColorCmyk::MapColorCmyk(float c, float m, float y, float k)
 : c(c), m(m), y(y), k(k) 
{
	// Nothing
}

inline
MapColorCmyk::MapColorCmyk(const MapColorCmyk& other)
 : c(other.c), m(other.m), y(other.y), k(other.k)
{
	// Nothing
}

inline
MapColorCmyk::MapColorCmyk(const QColor& other)
 : c(other.cyanF()), m(other.magentaF()), y(other.yellowF()), k(other.blackF())
{
	// Nothing
}

inline
void MapColorCmyk::operator=(const MapColorCmyk& other)
{
	c = other.c;
	m = other.m;
	y = other.y;
	k = other.k;
}

inline
MapColorCmyk::operator QColor() const
{
	return QColor::fromCmykF(c, m, y, k);
}

inline
bool MapColorCmyk::isBlack() const
{
	return (1.0 == k) || (1.0 == c && 1.0 == m && 1.0 == y);
}

inline
bool MapColorCmyk::isWhite() const
{
	return (0.0 == c && 0.0 == m && 0.0 == y && 0.0 == k);
}

inline
bool operator==(const MapColorCmyk& lhs, const MapColorCmyk& rhs)
{
	// The maximum difference of two floating point member values
	//  which are regarded as *equal*.
	static const float epsilon = 0.0005f;
	return ( qAbs(lhs.c - rhs.c) <= epsilon &&
	         qAbs(lhs.m - rhs.m) <= epsilon &&
	         qAbs(lhs.y - rhs.y) <= epsilon &&
	         qAbs(lhs.k - rhs.k) <= epsilon );
}

inline
bool operator!=(const MapColorCmyk& lhs, const MapColorCmyk& rhs)
{
	return !(lhs == rhs);
}


// ### MapColorRgb inline code ###

inline
MapColorRgb::MapColorRgb()
 : r(0.0f), g(0.0f), b(0.0f)
{
	Q_ASSERT(isBlack());
}

inline
MapColorRgb::MapColorRgb(float r, float g, float b)
 : r(r), g(g), b(b)
{
	// Nothing
}

inline
MapColorRgb::MapColorRgb(const MapColorRgb& other)
 : r(other.r), g(other.g), b(other.b)
{
	// Nothing
}

inline
MapColorRgb::MapColorRgb(const QColor& other)
 : r(other.redF()), g(other.greenF()), b(other.blueF())
{
	// Nothing
}

inline
void MapColorRgb::operator=(const MapColorRgb& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
}

inline
MapColorRgb::operator QColor() const
{
	return QColor::fromRgbF(r, g, b);
}

inline
bool MapColorRgb::isBlack() const
{
	return (0.0 == r && 0.0 == g && 0.0 == b);
}

inline
bool MapColorRgb::isWhite() const
{
	return (1.0 == r && 1.0 == g && 1.0 == b);
}

inline
bool operator==(const MapColorRgb& lhs, const MapColorRgb& rhs)
{
	// The maximum difference of two floating point member values
	//  which are regarded as *equal*.
	static const float epsilon = 0.0005f;
	return ( qAbs(lhs.r - rhs.r) <= epsilon &&
	         qAbs(lhs.g - rhs.g) <= epsilon &&
	         qAbs(lhs.b - rhs.b) <= epsilon );
}

inline
bool operator!=(const MapColorRgb& lhs, const MapColorRgb& rhs)
{
	return !(lhs == rhs);
}


// ### SpotColorComponent inline code ###

inline
SpotColorComponent::SpotColorComponent()
 : spot_color(NULL),
   factor(0.0f)
{
	// Nothing
}

inline
SpotColorComponent::SpotColorComponent(MapColor* spot_color, float factor)
 : spot_color(spot_color),
   factor(factor)
{
	// Nothing
}

inline
bool SpotColorComponent::isValid() const
{
	return spot_color != NULL;
}


// ### MapColor inline code ###

inline
MapColor::operator const QColor&() const
{
	return q_color;
}

inline
MapColor::operator QRgb() const
{
	return qRgba(qFloor(255.9 * cmyk.c), qFloor(255.9 * cmyk.m), qFloor(255.9 * cmyk.y), qFloor(255.9 * cmyk.k));
}

inline
const QString& MapColor::getName() const
{
	return name;
}

inline
void MapColor::setName(const QString& name)
{
	this->name = name;
}

inline
int MapColor::getPriority() const
{
	return priority;
}

inline
void MapColor::setPriority(int priority)
{
	this->priority = priority;
}

inline
float MapColor::getOpacity() const
{
	return opacity;
}

inline
void MapColor::setOpacity(float opacity)
{
	this->opacity = opacity;
}

inline
MapColor::ColorMethod MapColor::getSpotColorMethod() const
{
	return (ColorMethod)spot_color_method;
}

inline
const QString& MapColor::getSpotColorName() const
{
	return spot_color_name;
}

inline
const SpotColorComponents& MapColor::getComponents() const
{
	return components;
}

inline
MapColor::ColorMethod MapColor::getCmykColorMethod() const
{
	return (ColorMethod)cmyk_color_method;
}

inline
const MapColorCmyk& MapColor::getCmyk() const
{
	return cmyk;
}

inline
MapColor::ColorMethod MapColor::getRgbColorMethod() const
{
	return (ColorMethod)rgb_color_method;
}

inline
const MapColorRgb& MapColor::getRgb() const
{
	return rgb;
}

inline
bool MapColor::comparePriority(const MapColor& other) const
{
	return priority < other.priority;
}

inline
bool MapColor::equal(const MapColor* color, const MapColor* other)
{
	if (color == other)
		return true;
	else if (color && other)
		return color->equals(*other, false);
	else
		return false;
}

inline
bool operator==(const MapColor& lhs, const MapColor& rhs)
{
	return lhs.equals(rhs, true);
}

inline
bool operator!=(const MapColor& lhs, const MapColor& rhs)
{
	return !lhs.equals(rhs, true);
}

#endif