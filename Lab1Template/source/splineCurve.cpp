#include "splineCurve.h"

Spline::SplineSegment::SplineSegment(VectorXd coeff, float start, float end)
{
    coefficients = coeff;
    startX = start;
    endX = end;
}

// plug x into the equation of this segment
float Spline::SplineSegment::getY(float x)
{
    float sum = 0;
    for(int i = 0; i < 4; i++) 
    {
        sum += coefficients[i] * pow(x, i);
    }

    return sum;
}

// plug x into the derivative of this segment
float Spline::SplineSegment::getDY(float x)
{
    float sum = 0;
    sum += coefficients[1];
    sum += coefficients[2] * 2 * x;
    sum += coefficients[3] * 3 * pow(x, 2);

    return sum;
}


void Spline::SplineSegment::printSegment()
{
    cout << startX << " <= x >= " << endX << "   y = ";
    cout << coefficients[0] << " + ";
    cout << coefficients[1] << "x + ";
    cout << coefficients[2] << "x^2 + ";
    cout << coefficients[3] << "x^3";
    cout << "\n";
}

Spline::Spline()
{
}

Spline::Spline(vector<glm::vec2> points, float initSlope, float finalSlope)
{
    // currently magic
    int a = 0;
    int b = 1;
    int c = 2;
    int d = 3;
    int dimensions = (points.size() - 1) * 4;
    int row = 0;
    int i, j;
    // determine column with row and segment
    
    MatrixXd m(dimensions, dimensions);
    m.fill(0);
    VectorXd y(dimensions);
    y.fill(0);
    //VectorXd coefficients(dimensions);
    
    // slope at 0
    // 0a + 1b + 2cx + 3dx^2
    m(row, b) = 1;
    m(row, c) = 2 * points[0].x;
    m(row, d) = 3 * pow(points[0].x, 2);
    y(row) = initSlope;
    row++;

    for(i = 0; i < points.size() - 1; i++)
    {
        // first point
        m(row, a + i * 4) = 1;
        m(row, b + i * 4) = points[i].x;
        m(row, c + i * 4) = pow(points[i].x, 2);
        m(row, d + i * 4) = pow(points[i].x, 3);
        y(row) = points[i].y;
        row++;
    
        // last point
        m(row, a + i * 4) = 1;
        m(row, b + i * 4) = points[i + 1].x;
        m(row, c + i * 4) = pow(points[i + 1].x, 2);
        m(row, d + i * 4) = pow(points[i + 1].x, 3);
        y(row) = points[i + 1].y;
        row++;

        // if we are the last segment, finish differently
        if(i != points.size() - 2)
        {
            // slope at end of segment
            m(row, b + i * 4) = 1;
            m(row, c + i * 4) = 2 * points[i + 1].x;
            m(row, d + i * 4) = 3 * pow(points[i + 1].x, 2);
            m(row, b + (i + 1) * 4) = -1;
            m(row, c + (i + 1) * 4) = -2 * points[i + 1].x;
            m(row, d + (i + 1) * 4) = -3 * pow(points[i + 1].x, 2);
            y(row) = 0;
            row++;

            // curve at end of segment
            m(row, c + i * 4) = 2;
            m(row, d + i * 4) = 6 * points[i + 1].x;
            m(row, c + (i + 1) * 4) = -2;
            m(row, d + (i + 1) * 4) = -6 * points[i + 1].x;
            y(row) = 0;
            row++;
        }
    }

    // final slope
    m(row, b + (i - 1) * 4) = 1;
    m(row, c + (i - 1) * 4) = 2 * points[i].x;
    m(row, d + (i - 1) * 4) = 3 * pow(points[i].x, 2);
    y(row) = finalSlope;

    for(i = 0; i < dimensions; i++)
    {
        for(j = 0; j < dimensions; j++)
        {
            // set nans to 0 as well as excessive precision
            if(m(i, j) != m(i, j) || abs(m(i, j)) < 1e-5)
            {
                m(i, j) = 0;
            }
        }
    }

    coefficients = VectorXd(dimensions);
    coefficients = m.colPivHouseholderQr().solve(y);

    for(i = 0; i < points.size() - 1; i++)
    {
        VectorXd forSegment(4);
        for(int j = 0; j < 4; j++)
        {
            forSegment(j) = coefficients(j + i * 4);
        }

        pieces.push_back(SplineSegment(forSegment, points[i].x, points[i+1].x));
    }
}

float Spline::getY(float x)
{    
    for(int i = 0; i < pieces.size(); i++)
    {
        if(x >= pieces[i].startX && x <= pieces[i].endX)
        {
            return pieces[i].getY(x);
        }
    }

    //cout << "requested value was outside domain of spline\n";
    return -1000000; // bad value to ensure error is not overlooked
}

float Spline::getDY(float x)
{    
    for(int i = 0; i < pieces.size(); i++)
    {
        if(x >= pieces[i].startX && x <= pieces[i].endX)
        {
            return pieces[i].getDY(x);
        }
    }

    //cout << "requested value was outside domain of spline\n";
    return -1000000; // bad value to ensure error is not overlooked
}

void Spline::printSpline()
{
    cout << "This spline has " << pieces.size() << " pieces\n";
    for(int i = 0; i < pieces.size(); i++) {
        cout << "segment[" << i << "]: ";
        pieces[i].printSegment();
    }
}
