#include "GerenciadorSensores.h"

GerenciadorSensores::GerenciadorSensores() :
	mBarometro(),
	mBussola(),
	mGPS(),
	mIMU(),
	mSensorDistanciaFrente(),
	mSensorDistanciaBaixo(),
	mAltitude(0.0),
	mAnguloBussola(0.0),
	mLatitude(0.0),
	mLongitude(0.0),
	mRotacao({}),
	mVelocidadeAngular({}),
	mVelocidadeLinear({}),
	mDistanciaBaixo(0.0),
	mDistanciaFrente(0.0)
{

}

GerenciadorSensores::~GerenciadorSensores()
{

}

Int32
GerenciadorSensores::Iniciar()
{
	Int32 resultado = 0;

	resultado += mBarometro.Iniciar();
	resultado += mBussola.Iniciar();
	resultado += mGPS.Iniciar();
	resultado += mIMU.Iniciar();
	resultado += mSensorDistanciaFrente.Iniciar();
	resultado += mSensorDistanciaBaixo.Iniciar();

	return resultado;
}

Void
GerenciadorSensores::Calibrar()
{
	mBarometro.Calibrar();
	mBussola.Calibrar();
	mGPS.Calibrar();
	mIMU.Calibrar();
	mSensorDistanciaFrente.Calibrar();
	mSensorDistanciaBaixo.Calibrar();
}

Void 
GerenciadorSensores::Capturar()
{
	// Capturar sinais
	mBarometro.Capturar();
	mBussola.Capturar();
	mGPS.Capturar();
	mIMU.Capturar();
	mSensorDistanciaFrente.Capturar();
	mSensorDistanciaBaixo.Capturar();

	// Guardar informacoes
	mAltitude = mBarometro.Altitude();
	mAnguloBussola = mBussola.Angulo();
	mLatitude = mGPS.Latitude();
	mLongitude = mGPS.Longitude();
	mRotacao = mIMU.Rotacao();
	mVelocidadeAngular = mIMU.VelocidadeAngular();
	mVelocidadeLinear = mIMU.VelocidadeLinear();
	mDistanciaBaixo = mSensorDistanciaBaixo.Distancia();
	mDistanciaFrente = mSensorDistanciaFrente.Distancia();
}

Vetor3D
GerenciadorSensores::Posicao() const
{
	Vetor3D posicao = {
		mLatitude,
		mLongitude,
		mAltitude
	};

	return posicao;
}

Vetor3D
GerenciadorSensores::VelocidadeLinear() const
{
	return mVelocidadeLinear;
}

Vetor3D
GerenciadorSensores::Rotacao() const
{
	return mRotacao;
}

Vetor3D
GerenciadorSensores::VelocidadeAngular() const
{
	return mVelocidadeAngular;
}

Float 
GerenciadorSensores::Frente() const
{
	return mDistanciaFrente;
}
 
Float 
GerenciadorSensores::Baixo() const
{
	return mDistanciaBaixo;
}