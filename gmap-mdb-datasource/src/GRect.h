#ifndef __G_RECT_H_INCLUDED__
#define __G_RECT_H_INCLUDED__

#include "GTypes.h"
#include "GDimension2d.h"
#include "GPosition2d.h"

namespace gmap{
namespace rtree
{
	//!	Rectangle template.
	/** Mostly used by 2D GUI elements and for 2D drawing methods.
	    It has 2 positions instead of position and dimension
		and a fast method for collision detection with other rectangles and points.
	*/
	template <class T>
	class rect
	{
	public:

		rect()
			: UpperLeftCorner(0,0), LowerRightCorner(0,0) {};


		rect(T x, T y, T x2, T y2)
			: UpperLeftCorner(x,y), LowerRightCorner(x2,y2) {};


		rect(const position2d<T>& upperLeft, const position2d<T>& lowerRight)
			: UpperLeftCorner(upperLeft), LowerRightCorner(lowerRight) {};

		rect(const rect<T>& other)
			: UpperLeftCorner(other.UpperLeftCorner), LowerRightCorner(other.LowerRightCorner) {};

		rect(const position2d<T>& pos, const dimension2d<T>& size)
			: UpperLeftCorner(pos), LowerRightCorner(pos.X + size.Width, pos.Y + size.Height) {};


		rect<T> operator+(const position2d<T>& pos) const
		{
			rect<T> ret(*this);
			ret.UpperLeftCorner += pos;
			ret.LowerRightCorner += pos;
			return ret;
		}

		const rect<T>& operator+=(const position2d<T>& pos)
		{
			UpperLeftCorner += pos;
			LowerRightCorner += pos;
			return *this;
		}

		rect<T> operator-(const position2d<T>& pos) const
		{
			rect<T> ret(*this);
			ret.UpperLeftCorner -= pos;
			ret.LowerRightCorner -= pos;
			return ret;
		}

		const rect<T>& operator-=(const position2d<T>& pos)
		{
			UpperLeftCorner -= pos;
			LowerRightCorner -= pos;
			return *this;
		}

		bool operator == (const rect<T>& other) const
		{
			return (UpperLeftCorner == other.UpperLeftCorner &&
				LowerRightCorner == other.LowerRightCorner);
		}


		bool operator != (const rect<T>& other) const
		{
			return (UpperLeftCorner != other.UpperLeftCorner ||
				LowerRightCorner != other.LowerRightCorner);
		}

		const rect<T>& operator = (const rect<T>& other)
		{
			UpperLeftCorner = other.UpperLeftCorner;
			LowerRightCorner = other.LowerRightCorner;
			return *this;
		}
		template<class T2>
		const rect<T>& operator = (const rect<T2>& other)
		{
			UpperLeftCorner.X = (T)other.UpperLeftCorner.X;
			UpperLeftCorner.Y = (T)other.UpperLeftCorner.Y;
			LowerRightCorner.X = (T)other.LowerRightCorner.X;
			LowerRightCorner.Y = (T)other.LowerRightCorner.Y;
			return *this;
		}

		// compares size of rectangles
		bool operator < (const rect<T>& other) const
		{
			return getArea() < other.getArea();
		}

		//! Returns size of rectangle
		T getArea() const
		{
			return getWidth() * getHeight();
		}

		//! Returns if a 2d point is within this rectangle.
		//! \param pos: Position to test if it lies within this rectangle.
		//! \return Returns true if the position is within the rectangle, false if not.
		bool isPointInside(const position2d<T>& pos) const
		{
			return (UpperLeftCorner.X <= pos.X &&
				UpperLeftCorner.Y <= pos.Y &&
				LowerRightCorner.X >= pos.X &&
				LowerRightCorner.Y >= pos.Y);
		}

		//! Returns if the rectangle collides with another rectangle.
		bool isRectCollided(const rect<T>& other) const
		{
			return (LowerRightCorner.Y > other.UpperLeftCorner.Y &&
				UpperLeftCorner.Y < other.LowerRightCorner.Y &&
				LowerRightCorner.X > other.UpperLeftCorner.X &&
				UpperLeftCorner.X < other.LowerRightCorner.X);
		}

		//! Clips this rectangle with another one.
		void clipAgainst(const rect<T>& other) 
		{
			if (other.LowerRightCorner.X < LowerRightCorner.X)
				LowerRightCorner.X = other.LowerRightCorner.X;
			if (other.LowerRightCorner.Y < LowerRightCorner.Y)
				LowerRightCorner.Y = other.LowerRightCorner.Y;

			if (other.UpperLeftCorner.X > UpperLeftCorner.X)
				UpperLeftCorner.X = other.UpperLeftCorner.X;
			if (other.UpperLeftCorner.Y > UpperLeftCorner.Y)
				UpperLeftCorner.Y = other.UpperLeftCorner.Y;

			// correct possible invalid rect resulting from clipping
			if (UpperLeftCorner.Y > LowerRightCorner.Y)
				UpperLeftCorner.Y = LowerRightCorner.Y;
			if (UpperLeftCorner.X > LowerRightCorner.X)
				UpperLeftCorner.X = LowerRightCorner.X;
		}

		//! Moves this rectangle to fit inside another one.
		//! \return: returns true on success, false if not possible
		bool constrainTo(const rect<T>& other) 
		{
			if (other.getWidth() < getWidth() || other.getHeight() < getHeight())
				return false;

			T diff = other.LowerRightCorner.X - LowerRightCorner.X;
			if (diff < 0)
			{
				LowerRightCorner.X += diff;
				UpperLeftCorner.X  += diff;
			}

			diff = other.LowerRightCorner.Y - LowerRightCorner.Y;
			if (diff < 0)
			{
				LowerRightCorner.Y += diff;
				UpperLeftCorner.Y  += diff;
			}

			diff = UpperLeftCorner.X - other.UpperLeftCorner.X;
			if (diff < 0)
			{
				UpperLeftCorner.X  -= diff;
				LowerRightCorner.X -= diff;
			}

			diff = UpperLeftCorner.Y - other.UpperLeftCorner.Y;
			if (diff < 0)
			{
				UpperLeftCorner.Y  -= diff;
				LowerRightCorner.Y -= diff;
			}

			return true;
		}

		//! Returns width of rectangle.
		T getWidth() const
		{
			return LowerRightCorner.X - UpperLeftCorner.X;
		}

		//! Returns height of rectangle.
		T getHeight() const
		{
			return LowerRightCorner.Y - UpperLeftCorner.Y;
		}

		//! If the lower right corner of the rect is smaller then the
		//! upper left, the points are swapped.
		void repair()
		{
			if (LowerRightCorner.X < UpperLeftCorner.X)
			{
				T t = LowerRightCorner.X;
				LowerRightCorner.X = UpperLeftCorner.X;
				UpperLeftCorner.X = t;
			}

			if (LowerRightCorner.Y < UpperLeftCorner.Y)
			{
				T t = LowerRightCorner.Y;
				LowerRightCorner.Y = UpperLeftCorner.Y;
				UpperLeftCorner.Y = t;
			}
		}

		//! Returns if the rect is valid to draw. It could be invalid
		//! if the UpperLeftCorner is lower or more right than the
		//! LowerRightCorner, or if the area described by the rect is 0.
		bool isValid() const
		{
			// thx to jox for a correction to this method

			T xd = LowerRightCorner.X - UpperLeftCorner.X;
			T yd = LowerRightCorner.Y - UpperLeftCorner.Y;

			return !(xd <= 0 || yd <= 0 || (xd == 0 && yd == 0));
		}

		//! Returns the center of the rectangle
		position2d<T> getCenter() const
		{
			return position2d<T>((UpperLeftCorner.X + LowerRightCorner.X) / 2,
				(UpperLeftCorner.Y + LowerRightCorner.Y) / 2);
		}

		//! Returns the dimensions of the rectangle
		dimension2d<T> getSize() const
		{
			return dimension2d<T>(getWidth(), getHeight());
		}


		//! Adds a point to the rectangle, causing it to grow bigger, 
		//! if point is outside of the box
		//! \param p: Point to add into the box.
		void addInternalPoint(const position2d<T>& p)
		{
			addInternalPoint(p.X, p.Y);
		}

		//! Adds a point to the bounding rectangle, causing it to grow bigger, 
		//! if point is outside of the box.
		//! \param x: X Coordinate of the point to add to this box.
		//! \param y: Y Coordinate of the point to add to this box.
		void addInternalPoint(T x, T y)
		{
			if (x>LowerRightCorner.X) LowerRightCorner.X = x;
			if (y>LowerRightCorner.Y) LowerRightCorner.Y = y;

			if (x<UpperLeftCorner.X) UpperLeftCorner.X = x;
			if (y<UpperLeftCorner.Y) UpperLeftCorner.Y = y;
		}




		position2d<T> UpperLeftCorner;
		position2d<T> LowerRightCorner;
	};
}
} // 


#endif

