#include "ComplexPlane.h"

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	m_pixel_size.x = pixelWidth;
	m_pixel_size.y = pixelHeight;
	m_aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);
	m_plane_center.x = 0.0;
	m_plane_center.y = 0.0;
	m_plane_size.x = BASE_WIDTH;
	m_plane_size.y = BASE_HEIGHT * m_aspectRatio;
	m_zoomCount = 0;
	m_state = State::CALCULATING;
	m_vArray.setPrimitiveType(Points);
	m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}

void ComplexPlane::zoomIn()
{
	m_zoomCount++;

	float x_local = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y_local = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

	m_plane_size.x = x_local;
	m_plane_size.y = y_local;
	
	m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
	m_zoomCount--;

	float x_local = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y_local = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);

	m_plane_size.x = x_local;
	m_plane_size.y = y_local;

	m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	m_plane_center = mapPixelToCoords(mousePixel);

	m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
	ostringstream out_ss;

	out_ss << "Mandelbrot Set\n"
		<< "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n"
		<< "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n"
		<< "Left-click to zoom in\n"
		<< "Right-click to zoom out\n";

	text.setString(out_ss.str());
}

void ComplexPlane::updateRender()
{
	int iterations = 0;

	if (m_state == State::CALCULATING)
	{
		for (int i = 0; i < m_pixel_size.y; i++)
		{
			for (int j = 0; j < m_pixel_size.x; j++)
			{
				m_vArray[j + i * m_pixel_size.x].position = { (float)j, (float)i };

				Vector2f complexCoords = mapPixelToCoords(Vector2i(j, i));
				iterations = countIterations(complexCoords);

				Uint8 r, g, b;
				iterationsToRGB(iterations, r, g, b);

				m_vArray[j + i * m_pixel_size.x].color = { r, g, b };
			}
		}

		m_state = State::DISPLAYING;
	}
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
	/*
		Iterates through the Mandelbrot set until z is greater than 2, returning 
		the amount of iterations as an integer variable.
	*/

	size_t iterations = 0;
	float a_r = coord.x;
	float b_i = coord.y;

	complex<float>c(a_r, b_i);
	complex<float>z(0, 0);

	while (abs(z) <= 2.0 && iterations != MAX_ITER)
	{
		z = (z * z) + c;
		iterations++;
	}

	return iterations;
}


void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	/*
		Chooses the corresponding RGB values for a Value along the violet Hue (H = 260) 
		on the basis of how many iterations the Mandelbrot set went through.
	*/

	if (count == MAX_ITER)
	{
		r = 0;
		g = 0; 
		b = 0;
	}
	else if (count < 10)
	{
		r = 25; 
		g = 0; 
		b = 77;
	}
	else if (count >= 10 && count < 15)
	{
		r = 51;
		g = 0;
		b = 153;
	}
	else if (count >= 15 && count < 25)
	{
		r = 76;
		g = 0;
		b = 230;
	}
	else if (count >= 25 && count < 40)
	{
		r = 119;
		g = 51;
		b = 255;
	}
	else if (count >= 40 && count < 55)
	{
		r = 170;
		g = 128;
		b = 255;
	}
	else
	{
		r = 221;
		g = 204;
		b = 255;
	}
}


Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	/*
		Maps a pixel location on the user's monitor to a coordinate on the complex plane, 
		returning said coordinate as a Vector2f variable.
	*/

	Vector2f returnVector;
	returnVector.x = (((mousePixel.x - 0) / static_cast<float>(m_pixel_size.x - 0)) * (m_plane_size.x)
							+ (m_plane_center.x - m_plane_size.x / 2.0));
	returnVector.y = (((mousePixel.y - m_pixel_size.y) / static_cast<float>(0 - m_pixel_size.y)) * (m_plane_size.y)
							+ (m_plane_center.y - m_plane_size.y / 2.0));

	return returnVector;
}
