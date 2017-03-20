#ifndef KGVARY_H
#define KGVARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "KGMarco.h"

typedef void Surface;

typedef struct _KGPolygon_ {
	int num;
	KGPoint *point;
} KGPolygon;

Surface * __stdcall KGCreateGraph(KGColor color, KGSize size);
void __stdcall KGFreeGraph(Surface *face);
int __stdcall KGGraphSpin(Surface *face, int angle);
int __stdcall KGGraphScale(Surface *face, double scale_x, double scale_y);
int __stdcall KGDrawDot(Surface *face, KGColor color, KGPoint point);
int __stdcall KGDrawLine(Surface *face, KGColor color, KGPoint start, KGPoint end);
int __stdcall KGDrawCircle(Surface *face, KGColor color, KGPoint point, int radius);	//point is left top point
int __stdcall KGDrawCircleFill(Surface *face, KGColor color, KGPoint point, int radius);
void __stdcall KGMaollocPolygon(KGPolygon *polygon, int num);
/*
{
	if (polygon) {
		polygon->num = num;
		polygon->point = malloc(num * sizeof(KGPoint));
	}
}
*/
void __stdcall KGFreePolygon(KGPolygon *polygon);
/*
{
	if (polygon && polygon->point) {
		free(polygon->point);
		polygon = 0;
	}
}
*/
int __stdcall KGDrawPolygon(Surface *face, KGPolygon *polygon);
int __stdcall KGDrawPolygonFill(Surface *face, KGPolygon *polygon);
/*
	KGPolygon polygon;
	polygon.num = 4;
	KGMaollocPolygon(polygon, 4);
	polygon.point[0].x = 0;
	polygon.point[0].y = 0;
	polygon.point[1].x = 10;
	polygon.point[1].y = 10;
	.
	.
	KGDrawPolygon(face, polygon);
	KGFreePolygon(polygon);
*/
int __stdcall KGSaveGraph(const char *path, Surface *face);

#ifdef __cplusplus
}
#endif

#endif //KGVARY_H
