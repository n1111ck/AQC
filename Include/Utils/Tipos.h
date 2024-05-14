#ifndef _TIPOS_H_
#define _TIPOS_H_

typedef __int8 Int8;
typedef __int16 Int16;
typedef __int32 Int32;
typedef __int64 Int64;
typedef unsigned __int32 UInt8;
typedef unsigned __int32 UInt16;
typedef unsigned __int32 UInt32;
typedef unsigned __int32 UInt64;
typedef void Void;
typedef float Float;
typedef double Double;
typedef __int8 Boolean;

struct Vetor3D
{
	Float mX;
	Float mY;
	Float mZ;
};

struct Linha4D;

struct Vetor4D
{
	Float mW;
	Float mX;
	Float mY;
	Float mZ;

	Float Element(const UInt8& index) const;
	Void Saturar(const Float& inferior);
	Void Saturar(const Float& inferior, const Float& superior);
	Void Saturar(const Vetor4D& inferior, const Vetor4D& superior);
	Void Raiz();
	Float operator*(const Linha4D& linha) const;
	Vetor4D operator*(const Float& valor) const;

	Vetor4D operator-(const Vetor4D& vetor) const;
	Vetor4D operator*(const Vetor4D& vetor) const;
	Vetor4D operator+=(const Vetor4D& vetor);
	Vetor4D operator+=(const Float& valor);
	Vetor4D operator-=(const Float& valor);

};

struct Linha4D
{
	Float mW;
	Float mX;
	Float mY;
	Float mZ;

	Float Element(const UInt8& index) const;
	Void Saturar(const Float& inferior);
	Void Saturar(const Float& inferior, const Float& superior);
	Void Raiz();
	Float operator*(const Vetor4D& vetor) const;
	Linha4D operator*(const Float& valor) const;
};

struct ParametrosModelo
{
	Float mMassa;
	Float mRaio;
	Vetor3D mInercia;
	Float mPasso;
	Float mGravidade;
	Float mInerciaRotacao;
	Float mRelacaoVelocidade;
	Float mRelacaoForca;
	Float mRelacaoTorque;
	Float mConstanteTempo;
};

#endif // _TIPOS_H_