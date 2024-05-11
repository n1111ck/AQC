#include "Tipos.h"
#include "math.h"

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