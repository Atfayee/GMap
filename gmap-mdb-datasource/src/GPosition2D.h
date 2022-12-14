#ifndef __G_POSITION_H_INCLUDED__
#define __G_POSITION_H_INCLUDED__

#include "GTypes.h"
#include "GDimension2d.h"

namespace gmap
{
	namespace rtree{

		//! Simple class for holding 2d coordinates.
		/** Not supposed for doing geometric calculations.
		use vector2d instead for things like that. 
		*/
		template <class T>
		class position2d
		{
		public:
			position2d(T x, T y)
				: X(x), Y(y) {};


			position2d()
				: X(0), Y(0) {};


			position2d(const position2d<T>& other)
				: X(other.X), Y(other.Y) {};


			bool operator == (const position2d<T>& other) const
			{
				return X == other.X && Y == other.Y;
			}


			bool operator != (const position2d<T>& other) const
			{
				return X != other.X || Y != other.Y;
			}

			const position2d<T>& operator+=(const position2d<T>& other)
			{
				X += other.X;
				Y += other.Y;
				return *this;
			}

			const position2d<T>& operator-=(const position2d<T>& other)
			{
				X -= other.X;
				Y -= other.Y;
				return *this;
			}

			const position2d<T>& operator+=(const dimension2d<T>& other)
			{
				X += other.Width;
				Y += other.Height;
				return *this;
			}

			const position2d<T>& operator-=(const dimension2d<T>& other)
			{
				X -= other.Width;
				Y -= other.Height;
				return *this;
			}

			position2d<T> operator-(const position2d<T>& other) const
			{
				return position2d<T>(X-other.X, Y-other.Y);
			}

			position2d<T> operator+(const position2d<T>& other) const
			{
				return position2d<T>(X+other.X, Y+other.Y);
			}

			position2d<T> operator*(const position2d<T>& other) const
			{
				return position2d<T>(X*other.X, Y*other.Y);
			}

			position2d<T> operator*(const T& scalar) const
			{
				return position2d<T>(X*scalar, Y*scalar);
			}

			position2d<T> operator+(const dimension2d<T>& other) const
			{
				return position2d<T>(X+other.Width, Y+other.Height);
			}

			position2d<T> operator-(const dimension2d<T>& other) const
			{
				return position2d<T>(X-other.Width, Y-other.Height);
			}

			const position2d<T>& operator=(const position2d<T>& other) 
			{
				X = other.X;
				Y = other.Y;
				return *this;
			}

			T X, Y;
		};

		//! Typedef for a f32 position.
		typedef position2d<f32> position2df;
		//! Typedef for an integer position.
		typedef position2d<s32> position2di;
	}
} // end namespace gp

#endif

