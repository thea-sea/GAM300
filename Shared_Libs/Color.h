#ifndef COLOR_H
#define COLOR_H

namespace TDS
{
class ColorLinear;
class ColorGamma;
class Color
{
public:
	// Constructor
	DLL_API Color();
	DLL_API constexpr Color(float fLinearRed, float fLinearGreen, float fLinearBlue, float fLinearAlpha = 1.0f);
	DLL_API Color(const ColorLinear& cc);
	DLL_API Color(const ColorGamma& cc);
	void SetRGB(float fLinearRed, float fLinearGreen, float fLinearBlue);
	void SetRGBA(float fLinearRed, float fLinearGreen, float fLinearBlue, float fLinearAlpha = 1.0f);

	//Static Functions
	[[nodiscard]] static Color SetZero();
	[[nodiscard]] static Color SetNaN();

	// Helper Functions/Conversion
	[[nodiscard]] static Color SetHSV(float fHue, float fSat, float fVal);
	void GetHSV(float& fHue, float& fSat, float& fVal) const;
	const Vec4 GetAsVec4() const;
	
	static float GammaToLinear(float fGamma);
	static float LinearToGamma(float fLinear);

	static Vec3 GammaToLinear(const Vec3& vGamma);
	static Vec3 LinearToGamma(const Vec3& vLinear);

	// Internal Color functions
	bool IsNormalized() const;
	float CalcAverageRGB() const;
	float GetSaturation() const;
	float GetLuminance() const;

	Color GetInvertedColor() const;
	Color GetComplementaryColor() const;
	
	void ScaleRGB(float fFactor);
	
	float ComputeHdrMultiplier() const;
	float ComputeHdrExposureValue() const;
	void ApplyHdrExposureValue(float fEv);
	void NormalizeToLdrRange();
	
	Color GetDarker(float fFactor = 2.f) const;

	// Numeric Properties
	bool IsNaN() const;
	bool IsValid() const;

	// Operators
	void operator=(const ColorLinear& cc);
	void operator=(const ColorGamma& cc);
	void operator+=(const Color& rhs);
	void operator-=(const Color& rhs);
	void operator*=(const Color& rhs);
	void operator*=(float fScale);
	void operator/=(float fScale);
	void operator*=(const Mat4& rhs);
	
	float& operator[](int index);
	float const& operator[](int index) const;

	bool IsIdenticalRGB(const Color& rhs) const;
	bool IsIdenticalRGBA(const Color& rhs) const;
	
	bool IsEqualRGB(const Color& rhs, float fEpsilon) const;
	bool IsEqualRGBA(const Color& rhs, float fEpsilon) const;
	
	/*!*************************************************************************
	 * \brief	Returns the current Color but with changes to the alpha value
	 ***************************************************************************/
	Color WithAlpha(float fAlpha) const;

	// Data
	float r;
	float g;
	float b;
	float a;

};

// Operators
const Color operator+(const Color& c1, const Color& c2);
const Color operator-(const Color& c1, const Color& c2);

const Color operator*(const Color& c1, const Color& c2);
const Color operator*(const Color& c1, float fScale);
const Color operator*(float fScale, const Color& c1);

const Color operator/(const Color& c1, float fScale);
const Color operator*(const Mat4& lhs, const Color& rhs);

bool operator==(const Color& c1, const Color& c2);
bool operator!=(const Color& c1, const Color& c2);

/*!*************************************************************************
 * Strict weak ordering. Useful for sorting color into a map
 ***************************************************************************/
bool operator<(const Color& c1, const Color& c2);

} // namespace TDS


#endif // COLOR_H
