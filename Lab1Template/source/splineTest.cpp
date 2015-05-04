#include "splineCurve.h"

void splineTest()
{
//    cout << "testing spline curves\n";

    // get points for the curve
    vector<glm::vec2> points;
    points.push_back(glm::vec2(0, 1));
    points.push_back(glm::vec2(2, 2));
    points.push_back(glm::vec2(5, 0));
    points.push_back(glm::vec2(8, 0));
    points.push_back(glm::vec2(12, 5));
    points.push_back(glm::vec2(15, 6));
    points.push_back(glm::vec2(20, 0));

    Spline example = Spline(points, 0, 0);

    for(float x = 0; x <= 20; x += 0.1)
    {
        cout << x << ", " << example.getY(x) << "\n";
    }    

    exit(0);
}
