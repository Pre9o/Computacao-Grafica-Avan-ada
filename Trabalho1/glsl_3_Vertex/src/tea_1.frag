//Autor: Cesar Tadeu Pozzer

varying float cor1, cor2;
varying vec4 transformedPos;

void main()
{
    if(transformedPos.x > 0.0){ // Usa a posição transformada para determinar a cor
        gl_FragColor = vec4(cor1, 0, 0, 1.0);
    }
    else{
        gl_FragColor = vec4(0, cor2, 0, 1.0);
    }
}