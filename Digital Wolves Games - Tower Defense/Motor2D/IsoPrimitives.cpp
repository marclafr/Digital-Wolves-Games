#include "IsoPrimitives.h"

#include "j1App.h"
#include "j1Render.h"
#include <math.h>
#include "p2Log.h"

///Class Primitive ------------------------------
//Constructors ==============
Primitive::Primitive()
{}

Primitive::Primitive(const iPoint& position, const iPoint& displacement, const SDL_Color& color) :position(position), displacement(displacement), color(color)
{}

Primitive::Primitive(const Primitive& copy) : position(copy.position), displacement(copy.displacement), color(copy.color), x_angle(copy.x_angle)
{}

//Destructors ===============
Primitive::~Primitive()
{

}

//Functionality =============
bool Primitive::Draw()
{
	return true;
}

void Primitive::SetPosition(const iPoint & pos)
{
	position = pos;
}

void Primitive::SetDisplacement(const iPoint & desp)
{
	displacement = desp;
}

void Primitive::SetXAngle(float angle)
{
	x_angle = angle;
}

void Primitive::SetColor(const SDL_Color & rgba)
{
	color = rgba;
}

iPoint Primitive::GetPosition() const
{
	return position;
}
iPoint Primitive::GetDisplacement() const
{
	return displacement;
}
float Primitive::GetXAngle() const
{
	return x_angle;
}
SDL_Color Primitive::GetColor() const
{
	return color;
}
/// ---------------------------------------------


///Class Circle ---------------------------------
//Constructors ==============
Circle::Circle(const iPoint& position, uint rad, const iPoint& desplacement) :Primitive(position, desplacement), rad(rad)
{}

Circle::Circle(const Circle & copy) : Primitive(copy), rad(copy.rad)
{}

//Destructors ===============
Circle::~Circle()
{}

//Functionality =============
bool Circle::Draw()
{
	return App->render->DrawElipse(position.x + displacement.x, position.y + displacement.y, rad, color.r, color.g, color.b, color.a, x_angle, true);
}

bool Circle::IsIn(const fPoint* loc) const
{
	float sin_rad = rad * sin(x_angle);
	return ((((loc->x - position.x) * (loc->x - position.x)) / (rad  *rad) + ((loc->y - position.y) * (loc->y - position.y)) / (sin_rad * sin_rad)) <= 1);
}

bool Circle::Intersects(const SDL_Rect * rect) const
{
	if (rect == nullptr)return false;
	float dx = abs(position.x - (rect->x + rect->w * 0.5));
	float dy = abs(position.y - (rect->y + rect->h * 0.5));

	//Case the rect is out the circle limits in axis
	if (dx > rect->w * 0.5 + rad) return false;
	if (dy > rect->h * (0.5 + rad * sin(x_angle))) return false;

	//Case the rect is in the circle limits in axis
	if (dx <= rect->w * 0.5) return true;
	if (dx <= rect->w * 0.5) return true;

	//Case the rect intersects or not in the circle mid curves
	float a = (dx - rect->w * 0.5);
	float b = (dy - rect->h * 0.5);
	float corner_distance = a * a + b * b;

	return corner_distance <= rad ^ 2;
}

bool Circle::Intersects(const Circle * target) const
{
	if (target == nullptr)
		return false;
	iPoint vec(target->position.x - position.x, target->position.y - position.y);
	fPoint cpy(vec.x, vec.y);
	cpy.Norm();
	vec.x -= ((rad * cpy.x) + target->rad * cpy.x);
	vec.y -= (((target->rad + rad) * sin(x_angle) * cpy.y));
	float vec_len = sqrt(vec.x * vec.x + vec.y * vec.y);
	return (rad >= vec_len);
}
bool Circle::Overlap(const Circle* target) const
{
	if (target == nullptr)return false;

	int radius = rad + target->GetRad() + 1;
	int deltaX = position.x - target->GetPosition().x;
	int deltaY = position.y - target->GetPosition().y;

	return (abs(deltaX) < radius && abs(deltaY) < radius*sin(x_angle));
}
bool Circle::Intersects(const IsoRect * target) const
{
	if (target == nullptr)return false;
	iPoint vec = (target->GetPosition() + target->GetDisplacement()) - position;
	fPoint norm(vec.x, vec.y);
	norm.Norm();
	vec.x -= ceil((target->GetWidth() * 0.5) * norm.x);
	vec.y -= ceil(((target->GetHeight()) * 0.5) * norm.y);
	float len = sqrt(vec.x * vec.x + vec.y * vec.y);
	return (len <= rad);
}

bool Circle::Intersects(const iPoint* point) const
{
	if (point == nullptr)return false;
	iPoint loc(point->x - position.x, point->y - position.y);
	return (abs(loc.x) <= rad && abs(loc.y) <= rad);
}

iPoint Circle::NearestPoint(const Circle* target) const
{
	iPoint vec(target->position.x - position.x, target->position.y - position.y);
	fPoint cpy(vec.x, vec.y);
	cpy.Norm();
	vec.x -= ((rad * cpy.x) + target->rad * cpy.x);
	vec.y -= (((target->rad + rad) * sin(x_angle) * cpy.y));

	return iPoint(position.x + vec.x, position.y + vec.y);
}

iPoint Circle::NearestPoint(const IsoRect* target) const
{
	iPoint vec = (target->GetPosition() + target->GetDisplacement()) - position;
	fPoint norm(vec.x, vec.y);
	norm.Norm();
	vec.x -= ceil((target->GetWidth() * 0.5) * norm.x);
	vec.y -= ceil((target->GetHeight() * 0.5) * norm.y);
	return iPoint(position.x + vec.x, position.y + vec.y);
}

void Circle::SetRad(uint r)
{
	rad = r;
}

uint Circle::GetRad() const
{
	return rad;
}
/// ---------------------------------------------


///Class Rectangle ------------------------------
//Constructors ==============
IsoRect::IsoRect(const iPoint& position, float width, float height, const iPoint& desplacement) :Primitive(position, desplacement), width(width), height(height)
{}

IsoRect::IsoRect(const IsoRect& copy) : Primitive(copy), width(copy.width), height(copy.height)
{}

//Destructors ===============
IsoRect::~IsoRect()
{}

//Functionality =============
bool IsoRect::Draw() const
{
	//Draw lines with the correct angles and coordinates to form the rotated quad
	iPoint draw_pos(position.x + displacement.x, position.y + displacement.y);

	App->render->DrawLine(draw_pos.x - width * 0.5f, draw_pos.y, draw_pos.x, draw_pos.y - height*sin(x_angle), color.r, color.g, color.b, color.a, true);
	App->render->DrawLine(draw_pos.x - width * 0.5f, draw_pos.y, draw_pos.x, draw_pos.y + height*sin(x_angle), color.r, color.g, color.b, color.a, true);
	App->render->DrawLine(draw_pos.x + width * 0.5f, draw_pos.y, draw_pos.x, draw_pos.y - height*sin(x_angle), color.r, color.g, color.b, color.a, true);
	App->render->DrawLine(draw_pos.x + width * 0.5f, draw_pos.y, draw_pos.x, draw_pos.y + height*sin(x_angle), color.r, color.g, color.b, color.a, true);

	return true;
}

void IsoRect::SetWidth(uint w)
{
	width = w;
}

void IsoRect::SetHeight(uint h)
{
	height = h;
}

uint IsoRect::GetWidth() const
{
	return width;
}

uint IsoRect::GetHeight() const
{
	return height;
}

bool IsoRect::Inside(const iPoint pos) const
{
	/*int delta_x = abs(pos.x - position.x);
	if (delta_x > width / 2)
		return false;

	int delta_y = abs(pos.y - position.y);
	int result = delta_x * (2 * height * sin(x_angle));
	result = result / width;
	result += height * sin(x_angle);

	if (delta_y < result)
		return true;
	return false;
	*/
	float dx = abs(pos.x - position.x);
	float dy = abs(pos.y - position.y);
	if (dx / width + dy / height <= 1)
		return true;

	return false;
}
bool IsoRect::Overlaps(SDL_Rect rect) const
{
	//check if SDL_Rect vertices are inside IsoRect
	if (Inside(iPoint(rect.x, rect.y)))
		return true;
	if (Inside(iPoint(rect.x + rect.w, rect.y)))
		return true;
	if (Inside(iPoint(rect.x, rect.y + rect.h)))
		return true;
	if (Inside(iPoint(rect.x + rect.w, rect.y + rect.h)))
		return true;

	//check if IsoRect vertices are inside SDL_Rect 
	iPoint up(position.x, position.y - height / 2);
	if (up.x > rect.x && up.x < rect.x + rect.w
		&& up.y > rect.y && up.y < rect.y + rect.h)
		return true;

	iPoint left(position.x - width / 2, position.y);
	if (left.x > rect.x && left.x < rect.x + rect.w
		&& left.y > rect.y && left.y < rect.y + rect.h)
		return true;

	iPoint right(position.x + width / 2, position.y);
	if (right.x > rect.x && right.x < rect.x + rect.w
		&& right.y > rect.y && right.y < rect.y + rect.h)
		return true;

	iPoint down(position.x, position.y + height / 2);
	if (down.x > rect.x && down.x < rect.x + rect.w
		&& down.y > rect.y && down.y < rect.y + rect.h)
		return true;

	return false;
}
bool IsoRect::Overlaps(Circle circle) const
{
	return circle.Intersects(this);
}
/// ---------------------------------------------

///Class PivotedRect ----------------------------
//Constructors ==============
PivotedRect::PivotedRect(const iPoint & origin, const iPoint & goal, uint width, uint height) :Primitive(origin), goal(goal), width(width), height(height)
{

}

PivotedRect::PivotedRect(const PivotedRect & copy) : Primitive(copy), goal(copy.goal), width(copy.width), height(copy.height), pivot_distance(copy.pivot_distance)
{

}

//Destructors ===============
PivotedRect::~PivotedRect()
{

}

//Functionality =============
bool PivotedRect::Draw()
{
	App->render->DrawLine(vertex[0].x, vertex[0].y, vertex[1].x, vertex[1].y, color.r, color.g, color.b, color.a);
	App->render->DrawLine(vertex[2].x, vertex[2].y, vertex[0].x, vertex[0].y, color.r, color.g, color.b, color.a);
	App->render->DrawLine(vertex[2].x, vertex[2].y, vertex[3].x, vertex[3].y, color.r, color.g, color.b, color.a);
	App->render->DrawLine(vertex[3].x, vertex[3].y, vertex[1].x, vertex[1].y, color.r, color.g, color.b, color.a);

	return true;
}

void PivotedRect::SetGoal(const iPoint & new_goal)
{
	goal = new_goal;
}

void PivotedRect::SetWidth(uint new_width)
{
	width = new_width;
}

void PivotedRect::SetHeight(uint new_height)
{
	height = new_height;
}

void PivotedRect::SetPivotDistance(uint new_pivot_distance)
{
	pivot_distance = new_pivot_distance;
}

void PivotedRect::CalculateVertex()
{
	/*
	A-----G-----B
	-			-
	-			-
	-			-
	-			-
	-			-
	C-----M-----D
	-         -
	-   d   -
	-     -
	-   -
	- -
	P
	P is the pivot
	d is the pivot distance
	G is the goal
	*/

	//Calculate P -> G vector
	fPoint dir_vector(goal.x - this->position.x, goal.y - this->position.y);
	dir_vector.Norm();



	fPoint side_vec;
	side_vec.x = width * -dir_vector.y;
	side_vec.y = width * dir_vector.x * sin(x_angle);

	iPoint M_point = position;
	M_point.x += pivot_distance * dir_vector.x;
	M_point.y += pivot_distance * dir_vector.y * sin(x_angle);

	iPoint G_Point = M_point;
	G_Point.x += height * dir_vector.x;
	G_Point.y += height * dir_vector.y;

	//Calculate A vertex
	vertex[0].x = G_Point.x - side_vec.x;
	vertex[0].y = G_Point.y - side_vec.y;

	//Calculate B vertex
	vertex[1].x = G_Point.x + side_vec.x;
	vertex[1].y = G_Point.y + side_vec.y;

	//Calculate C vertex
	vertex[2].x = M_point.x - side_vec.x;
	vertex[2].y = M_point.y - side_vec.y;

	//Calculate D vertex
	vertex[3].x = M_point.x + side_vec.x;
	vertex[3].y = M_point.y + side_vec.y;


}

bool PivotedRect::IsIn(const fPoint* loc) const
{
	float rect_area = width * height;

	// AB
	float area_tri_AB = (vertex[0].x*(vertex[1].y - loc->y) + vertex[1].x * (loc->y - vertex[0].y) + loc->x * (vertex[0].y - vertex[1].y)) * 0.5* sin(x_angle);
	// DB
	float area_tri_DB = (vertex[3].x*(vertex[1].y - loc->y) + vertex[1].x * (loc->y - vertex[3].y) + loc->x * (vertex[3].y - vertex[1].y)) * 0.5* sin(x_angle);
	// AC
	float area_tri_AC = (vertex[0].x*(vertex[2].y - loc->y) + vertex[2].x * (loc->y - vertex[0].y) + loc->x * (vertex[0].y - vertex[2].y)) * 0.5* sin(x_angle);
	// DC
	float area_tri_DC = (vertex[3].x*(vertex[2].y - loc->y) + vertex[2].x * (loc->y - vertex[3].y) + loc->x * (vertex[3].y - vertex[2].y)) * 0.5* sin(x_angle);

	return (!((abs(area_tri_AB) + abs(area_tri_AC) + abs(area_tri_DB) + abs(area_tri_DC)) > rect_area));

}
bool PivotedRect::Intersects(const SDL_Rect* rect) const
{
	if (rect == nullptr)return false;
	iPoint n_vertex[4];
	for (uint k = 0; k < 4; k++)n_vertex[k] = vertex[k];

	if (SDL_IntersectRectAndLine(rect, &n_vertex[0].x, &n_vertex[0].y, &n_vertex[1].x, &n_vertex[1].y))return true;
	if (SDL_IntersectRectAndLine(rect, &n_vertex[0].x, &n_vertex[0].y, &n_vertex[2].x, &n_vertex[2].y))return true;
	if (SDL_IntersectRectAndLine(rect, &n_vertex[2].x, &n_vertex[2].y, &n_vertex[3].x, &n_vertex[3].y))return true;
	if (SDL_IntersectRectAndLine(rect, &n_vertex[3].x, &n_vertex[3].y, &n_vertex[1].x, &n_vertex[1].y))return true;
	return false;
}
uint PivotedRect::GetWidth()const
{
	return width;
}
uint PivotedRect::GetHeight()const
{
	return height;
}
uint PivotedRect::GetPivotDistance() const
{
	return pivot_distance;
}
/// ---------------------------------------------
///Class Line -----------------------------------
//Constructors ==============
Line::Line(const iPoint & position, const iPoint & position_2, const SDL_Color& color, const iPoint& desplacement) :Primitive(position, desplacement, color), position_2(position_2)
{

}

Line::Line(const IsoRect & copy) : Primitive(position, displacement, color), position_2(position_2)
{
}

//Destructors ===============
Line::~Line()
{
}

//Functionality =============
bool Line::Draw()
{
	return App->render->DrawLine(position.x + displacement.x, position.y + displacement.y, position_2.x, position_2.y, color.r, color.g, color.b, color.a);
}

void Line::SetP1(const iPoint & p1)
{
	position = p1;
}

void Line::SetP2(const iPoint & p2)
{
	position_2 = p2;
}

const iPoint& Line::GetP1() const
{
	return position;
}

const iPoint& Line::GetP2() const
{
	return position_2;
}