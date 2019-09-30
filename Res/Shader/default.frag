#version 330 core
#define MAX_POINT_LIGHTS 2

struct DirectionalLight
{
	vec3 Direction;
	vec3 Color;
	sampler2D ShadowBuffer;
	mat4 ViewProjection;
};

struct PointLight
{
	vec3 Position;
	vec3 Color;
	float Radius;
};

struct SpotLight
{
	vec3 Position;
	vec3 Direction;
	vec3 Color;
	float Length;
	float Angle;
};


uniform sampler2D u_MainTexture;
uniform float u_Time;
uniform vec3 u_EyePosition;
uniform DirectionalLight u_DirLight;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform SpotLight u_SpotLight;

in vec3 f_Normal;
in vec2 f_TexCoord;
in vec3 f_World;
in float f_CameraDepth;
out vec4 o_Color;

const vec3 AmbientColor = vec3(1.0, 0.8, 0.2);
const float SpecExponent = 18.0;
const float SpecIntensity = 0.2;
const float ShadowBias = 0.0001;

const float offset = 1.0 / 519.0;

const float AttenuationExponent = 0.00001;
const float AttenuationLinear = 0.1;
const float AttenuationQuadratic = 0.05;
const float AttenuationConstant = 1.0;

const float SpotLightFalloff = radians(5.f);

const vec3 FogColor = vec3(0.9, 0.2, 0.2);
const float FogMinDistance = 20.0;
const float FogDistance = 80.0;

vec3 CalculateDirLightColor(DirectionalLight Light, vec3 Albedo)
{
	//Calculate Light
	//Diffuse Lightning
	vec3 Diffuse = Albedo * Light.Color * max(-dot(Light.Direction, f_Normal), 0.0);
	

	//Specular light

	vec3 WorldToEye = normalize(u_EyePosition - f_World);
	vec3 HalfWayVector = normalize(WorldToEye - Light.Direction);
	float SpecularValue = max(dot(HalfWayVector, f_Normal), 0.0);
	SpecularValue = pow(SpecularValue, SpecExponent) * SpecIntensity;

	vec3 Specular = Light.Color * SpecularValue;

	//Blinn-Phong Model
	//Everything is shifted from -1, 1 to 0, 1 in light buffer

	//Shadows baby
	vec4 LightNDC = Light.ViewProjection * vec4(f_World, 1.0);

	LightNDC = LightNDC * 0.5 + 0.5;
	float LightDepth = texture(Light.ShadowBuffer, LightNDC.xy).x;
	float OurDepth = LightNDC.z;
	float Shadow = step(OurDepth, LightDepth + ShadowBias);

	Diffuse *= Shadow;
	Specular *= Shadow;
	
	return Diffuse + Specular;
}

vec3 CalculatePointLightColor(PointLight Light, vec3 Albedo)
{
	vec3 LightDirection = normalize(f_World - Light.Position);

	float Distance = length(f_World - Light.Position);
	//POLYNOMIA ATTENUATION
	//float Attenuation = 1.0 / (Distance * Distance * AttenuationQuadratic + Distance * AttenuationLinear + AttenuationConstant);
	
	//exponential attenuation
	float Attenuation = max(1.0 - (Distance / Light.Radius), 0.0);
	Attenuation = pow(Attenuation, AttenuationExponent);

	vec3 Diffuse = Albedo * Light.Color * Attenuation * max(-dot(LightDirection, f_Normal), 0.0);

	//Specular Highlitning

	vec3 WorldToEye = normalize(u_EyePosition - f_World);
	vec3 HalfVector = normalize(WorldToEye - LightDirection);
	float SpecularValue = max(dot(HalfVector, f_Normal), 0.0);
	SpecularValue = pow(SpecularValue, SpecExponent) * SpecIntensity * Attenuation;

	vec3 Specular = Light.Color * Attenuation * SpecularValue;

	return Diffuse + Specular;
}

vec3 CalculateSpotLightColor(SpotLight Light, vec3 Albedo)
{
	vec3 LightDirection = normalize(f_World - Light.Position);

	float Distance = length(f_World - Light.Position);
	//POLYNOMIA ATTENUATION
	//float Attenuation = 1.0 / (Distance * Distance * AttenuationQuadratic + Distance * AttenuationLinear + AttenuationConstant);
	
	//exponential attenuation
	float Attenuation = max(1.0 - (Distance / Light.Length), 0.0);
	Attenuation = pow(Attenuation, AttenuationExponent);

	vec3 Diffuse = Albedo * Light.Color * Attenuation * max(-dot(LightDirection, f_Normal), 0.0);

	//Specular Highlitning

	vec3 WorldToEye = normalize(u_EyePosition - f_World);
	vec3 HalfVector = normalize(WorldToEye - LightDirection);
	float SpecularValue = max(dot(HalfVector, f_Normal), 0.0);
	SpecularValue = pow(SpecularValue, SpecExponent) * SpecIntensity * Attenuation;
	vec3 Specular = Light.Color * Attenuation * SpecularValue;

	//Angle cutoff
	float CosAngle = dot(LightDirection, Light.Direction);
	float Angle = acos(CosAngle);
	float Cutoff = 1.0 - smoothstep(Light.Angle - SpotLightFalloff, Light.Angle, Angle);

	return (Diffuse + Specular) * Cutoff;
}

void main(){

	 vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), 
        vec2( 0.0f,    offset),
        vec2( offset,  offset), 
        vec2(-offset,  0.0f),   
        vec2( 0.0f,    0.0f),   
        vec2( offset,  0.0f),   
        vec2(-offset, -offset), 
        vec2( 0.0f,   -offset),
        vec2( offset, -offset)     
    );

	float kernel[9] = float[](
	1.0 / 16, 2.0 / 16, 1.0 / 16,
	2.0 / 16, 4.0 / 16, 2.0 / 16,
	1.0 / 16, 2.0 / 16, 1.0 / 16

);

vec3 MySamplerTex[9];
for(int i = 0; i < 9; i++)
{
	MySamplerTex[i] = vec3(texture(u_MainTexture, f_TexCoord.st + offsets[i]));
}

vec3 columns = vec3(0.0);
for(int i = 0; i < 9; i++)
{
	columns += MySamplerTex[i] * kernel[i];
}

	o_Color = vec4(columns, 0.5);
	
	

    vec3 Albedo = texture(u_MainTexture, f_TexCoord).xyz;

	//Ambient light
	vec3 Ambient = AmbientColor * Albedo * 0.2;

	//Apply Light Sources
	o_Color = vec4(0.0, 0.0, 0.0, 1.0);
	o_Color.xyz += CalculateDirLightColor(u_DirLight, Albedo);

	for(int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		o_Color.xyz += CalculatePointLightColor(u_PointLights[i], Albedo);

	}
	o_Color.xyz += CalculateSpotLightColor(u_SpotLight, Albedo);
	o_Color.xyz += Ambient;


	//GrayScale
	o_Color = texture(u_MainTexture, f_TexCoord);
	float average = 0.2126 * o_Color.r + 0.7152 * o_Color.g + 0.0722 * o_Color.b;
	o_Color = vec4(average, average, average, 1.0);

	//Iversion
	//o_Color = vec4(vec3(1.0 - texture(u_MainTexture, f_TexCoord)), 1.0);

	//Apply Fog to the lit pixel
	float FogIntensity = clamp((f_CameraDepth - FogMinDistance) / FogDistance, 0.0, 1.0);
	FogIntensity = pow(FogIntensity, 0.94);
	o_Color.xyz = mix(o_Color.xyz, FogColor, FogIntensity);

}