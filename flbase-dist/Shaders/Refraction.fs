#version 120

struct Light
{
    vec3 direction;
};

struct Material
{
    bool isUsingCubemap;
    vec4 diffuse;
    bool enableRefraction;
    float refractiveIndex;
};

uniform Light light;
uniform Material material;
uniform samplerCube environment;

varying vec3 vVertex;
varying vec3 vNormal;
varying vec3 vEyeDirection;

void computeRefraction( in vec3 incom, in vec3 normal, in float indexExternal, in float indexInternal,
                        out vec3 reflection, out vec3 refraction,
                        out float reflectance, out float transmittance )
{
    float eta = indexExternal / indexInternal;
    float cosTheta1 = max( 0, dot( incom, normal ) );
    float disc = max( 0, 1.0 - ( ( eta * eta ) * ( 1.0 - ( cosTheta1 * cosTheta1 ) ) ) );

    float cosTheta2 = sqrt( disc );
    reflection = 2.0 * cosTheta1 * normal - incom;
    refraction = ( eta * cosTheta1 - cosTheta2 ) * normal - ( eta * incom );

    float fresnelRS = max( 0, ( indexExternal * cosTheta1 - indexInternal * cosTheta2 ) / ( indexExternal * cosTheta1 + indexInternal * cosTheta2 ) );
    float fresnelRP = max( 0, ( indexInternal * cosTheta1 - indexExternal * cosTheta2 ) / ( indexInternal * cosTheta1 + indexExternal * cosTheta2 ) );

    reflectance = ( fresnelRS * fresnelRS + fresnelRP * fresnelRP ) / 2.0;
    transmittance =  ( ( 1.0 - fresnelRS ) * ( 1.0 - fresnelRS ) + ( 1.0 - fresnelRP ) * ( 1.0 - fresnelRP ) ) / 2.0;
}

void main()
{
    gl_FragColor = vec4(0,0,0,1);
    vec3 N = normalize( vNormal );

    if ( material.isUsingCubemap )
    {
        // Cubemap
        gl_FragColor = textureCube( environment, vVertex );
    }
    else
    {
        // Diffuse
        float nDotD = abs( dot( N, light.direction ) );
        gl_FragColor.rgb = material.diffuse.rgb * nDotD;
        gl_FragColor.a = material.diffuse.a;
    }

    // Refract
    if ( material.enableRefraction )
    {
        vec3 incom = normalize( vEyeDirection );
        vec3 refractionRay, reflectionRay;
        float fresnelR, fresnelT;

        computeRefraction( incom, N, 1.0, material.refractiveIndex, reflectionRay, refractionRay, fresnelR, fresnelT );


        vec4 reflectColor = textureCube( environment, reflectionRay ) * fresnelR;
        vec4 refractColor = textureCube( environment, refractionRay ) * fresnelT;
        gl_FragColor.rgb += reflectColor.rgb + refractColor.rgb;

        // Artificial diffuse
        float nDotD = abs( dot( N, light.direction ) );
        gl_FragColor.rgb = gl_FragColor.rgb * 0.9 + vec3(0, 0, 0.1) * ( 1 - nDotD );

    }
}

