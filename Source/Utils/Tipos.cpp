#include "Utils/Tipos.h"
#include "math.h"

namespace AQC
{
	Vetor3D::Vetor3D():
		mX(0.0),
		mY(0.0),
		mZ(0.0)
	{

	}

	Vetor3D::Vetor3D(
		const Float& x,
		const Float& y,
		const Float& z
	) :
		mX(x),
		mY(y),
		mZ(z)
	{

	}

	Vetor4D::Vetor4D() :
		mW(0.0),
		mX(0.0),
		mY(0.0),
		mZ(0.0)
	{

	}

	Vetor4D::Vetor4D(
		const Float& w,
		const Float& x,
		const Float& y,
		const Float& z
	):
		mW(w),
		mX(x),
		mY(y),
		mZ(z)
	{

	}

	Float
	Vetor4D::Element(const UInt8& index) const
	{
		Float resultado = 0.0;

		switch (index)
		{
		case 0:
			resultado = mW;
			break;
		case 1:
			resultado = mX;
			break;
		case 2:
			resultado = mY;
			break;
		case 3:
			resultado = mZ;
			break;
		default:
			resultado = 0.0;
			break;
		}

		return resultado;
	}

	Float
	Vetor4D::operator*(const Linha4D& linha) const
	{
		return (linha.mW * mW +
			linha.mX * mX +
			linha.mY * mY +
			linha.mZ * mZ
			);
	}

	Void
	Vetor4D::Saturar(const Float& inferior)
	{
		if (mW < inferior)
		{
			mW = inferior;
		}

		if (mX < inferior)
		{
			mX = inferior;
		}

		if (mY < inferior)
		{
			mY = inferior;
		}

		if (mZ < inferior)
		{
			mZ = inferior;
		}
	}

	Void
	Vetor4D::Saturar(const Float& inferior, const Float& superior)
	{
		if (mW < inferior)
		{
			mW = inferior;
		}
		else if (mW > superior)
		{
			mW = superior;
		}

		if (mX < inferior)
		{
			mX = inferior;
		}
		else if (mX > superior)
		{
			mX = superior;
		}

		if (mY < inferior)
		{
			mY = inferior;
		}
		else if (mY > superior)
		{
			mY = superior;
		}

		if (mZ < inferior)
		{
			mZ = inferior;
		}
		else if (mZ > superior)
		{
			mZ = superior;
		}
	}

	Void
	Vetor4D::Saturar(const Vetor4D& inferior, const Vetor4D& superior)
	{
		if (mW < inferior.mW)
		{
			mW = inferior.mW;
		}
		else if (mW > superior.mW)
		{
			mW = superior.mW;
		}

		if (mX < inferior.mX)
		{
			mX = inferior.mX;
		}
		else if (mX > superior.mX)
		{
			mX = superior.mX;
		}

		if (mY < inferior.mY)
		{
			mY = inferior.mY;
		}
		else if (mY > superior.mY)
		{
			mY = superior.mY;
		}

		if (mZ < inferior.mZ)
		{
			mZ = inferior.mZ;
		}
		else if (mZ > superior.mZ)
		{
			mZ = superior.mZ;
		}
	}

	Void
	Vetor4D::Raiz()
	{
		mW = pow(mW, 0.5);
		mX = pow(mX, 0.5);
		mY = pow(mY, 0.5);
		mZ = pow(mZ, 0.5);
	}

	Vetor4D
	Vetor4D::operator*(const Float& valor) const
	{
		Vetor4D resultado = *this;

		resultado.mW *= valor;
		resultado.mX *= valor;
		resultado.mY *= valor;
		resultado.mZ *= valor;

		return resultado;
	}

	Vetor4D 
	Vetor4D::operator/(const Float& valor) const
	{
		Vetor4D resultado = *this;

		resultado.mW /= valor;
		resultado.mX /= valor;
		resultado.mY /= valor;
		resultado.mZ /= valor;

		return resultado;
	}

	Vetor4D
	Vetor4D::operator-(const Vetor4D& vetor) const
	{
		Vetor4D resultado = *this;

		resultado.mW -= vetor.mW;
		resultado.mX -= vetor.mX;
		resultado.mY -= vetor.mY;
		resultado.mZ -= vetor.mZ;

		return resultado;
	}

	Vetor4D
	Vetor4D::operator*(const Vetor4D& vetor) const
	{
		Vetor4D resultado = *this;

		resultado.mW *= vetor.mW;
		resultado.mX *= vetor.mX;
		resultado.mY *= vetor.mY;
		resultado.mZ *= vetor.mZ;

		return resultado;
	}

	Vetor4D
	Vetor4D::operator+=(const Vetor4D& vetor)
	{
		mW += vetor.mW;
		mX += vetor.mX;
		mY += vetor.mY;
		mZ += vetor.mZ;

		return *this;
	}

	Vetor4D
	Vetor4D::operator+=(const Float& valor)
	{
		mW += valor;
		mX += valor;
		mY += valor;
		mZ += valor;

		return *this;
	}

	Vetor4D
	Vetor4D::operator-=(const Float& valor)
	{
		return operator+=(-valor);
	}

	Linha4D::Linha4D() :
		mW(0.0),
		mX(0.0),
		mY(0.0),
		mZ(0.0)
	{

	}

	Linha4D::Linha4D(
		const Float& w,
		const Float& x,
		const Float& y,
		const Float& z
	) :
		mW(w),
		mX(x),
		mY(y),
		mZ(z)
	{

	}

	Float
	Linha4D::Element(const UInt8& index) const
	{
		Float resultado = 0.0;

		switch (index)
		{
		case 0:
			resultado = mW;
			break;
		case 1:
			resultado = mX;
			break;
		case 2:
			resultado = mY;
			break;
		case 3:
			resultado = mZ;
			break;
		default:
			resultado = 0.0;
			break;
		}

		return resultado;
	}

	Float
	Linha4D::operator*(const Vetor4D& vetor) const
	{
		return (vetor.mW * mW +
			vetor.mX * mX +
			vetor.mY * mY +
			vetor.mZ * mZ
			);
	}

	Void
	Linha4D::Saturar(const Float& inferior)
	{
		if (mW < inferior)
		{
			mW = inferior;
		}

		if (mX < inferior)
		{
			mX = inferior;
		}

		if (mY < inferior)
		{
			mY = inferior;
		}

		if (mZ < inferior)
		{
			mZ = inferior;
		}
	}

	Void
	Linha4D::Saturar(const Float& inferior, const Float& superior)
	{
		if (mW < inferior)
		{
			mW = inferior;
		}
		else if (mW > superior)
		{
			mW = superior;
		}

		if (mX < inferior)
		{
			mX = inferior;
		}
		else if (mX > superior)
		{
			mX = superior;
		}

		if (mY < inferior)
		{
			mY = inferior;
		}
		else if (mY > superior)
		{
			mY = superior;
		}

		if (mZ < inferior)
		{
			mZ = inferior;
		}
		else if (mZ > superior)
		{
			mZ = superior;
		}
	}

	Void
	Linha4D::Raiz()
	{
		mW = pow(mW, 0.5);
		mX = pow(mX, 0.5);
		mY = pow(mY, 0.5);
		mZ = pow(mZ, 0.5);
	}

	Linha4D
	Linha4D::operator*(const Float& valor) const
	{
		Linha4D resultado = *this;

		resultado.mW *= valor;
		resultado.mX *= valor;
		resultado.mY *= valor;
		resultado.mZ *= valor;

		return resultado;
	}
}