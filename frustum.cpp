#include "frustum.h"
int frustum::boxInFrustum(Vec3f min, Vec3f max)
{
    int totalPointsInPlane=0;
    Vec3f vCorner[8];

    vCorner[0] = min;
    vCorner[1] = max;
    vCorner[2] = Vec3f(max[0], min[1], min[2]);
    vCorner[3] = Vec3f(max[0], max[1], min[2]);
    vCorner[4] = Vec3f(max[0], min[1], max[2]);
    vCorner[5] = Vec3f(min[0], max[1], max[2]);
    vCorner[6] = Vec3f(min[0], min[1], max[2]);
    vCorner[7] = Vec3f(min[0], max[1], min[2]);

    for(int p = 0; p < 6; ++p)
    {

		int behindPlaneCnt = 8;
		int numPointsInPlane = 1;

		for(int i = 0; i < 8; ++i)
			if(fPlanes[p].SideOfPlane(vCorner[i]) == BEHIND)
			{
                numPointsInPlane = 0;
				--behindPlaneCnt;
			}

		if(behindPlaneCnt == 0)
			return OUT;

		totalPointsInPlane += numPointsInPlane;
	}

	if(totalPointsInPlane == 6)
		return FULL;

	return PARTIAL;

}

void frustum::UpdateFrustum()
{
    float fProj[16];
    float fView[16];
    float fClip[16];
    float fT;

    glGetFloatv(GL_PROJECTION_MATRIX, fProj);
    glGetFloatv(GL_MODELVIEW_MATRIX, fView);

    fClip[ 0] = fView[ 0] * fProj[ 0] + fView[ 1] * fProj[ 4] + fView[ 2] * fProj[ 8] + fView[ 3] * fProj[12];
    fClip[ 1] = fView[ 0] * fProj[ 1] + fView[ 1] * fProj[ 5] + fView[ 2] * fProj[ 9] + fView[ 3] * fProj[13];
    fClip[ 2] = fView[ 0] * fProj[ 2] + fView[ 1] * fProj[ 6] + fView[ 2] * fProj[10] + fView[ 3] * fProj[14];
    fClip[ 3] = fView[ 0] * fProj[ 3] + fView[ 1] * fProj[ 7] + fView[ 2] * fProj[11] + fView[ 3] * fProj[15];

    fClip[ 4] = fView[ 4] * fProj[ 0] + fView[ 5] * fProj[ 4] + fView[ 6] * fProj[ 8] + fView[ 7] * fProj[12];
    fClip[ 5] = fView[ 4] * fProj[ 1] + fView[ 5] * fProj[ 5] + fView[ 6] * fProj[ 9] + fView[ 7] * fProj[13];
    fClip[ 6] = fView[ 4] * fProj[ 2] + fView[ 5] * fProj[ 6] + fView[ 6] * fProj[10] + fView[ 7] * fProj[14];
    fClip[ 7] = fView[ 4] * fProj[ 3] + fView[ 5] * fProj[ 7] + fView[ 6] * fProj[11] + fView[ 7] * fProj[15];

    fClip[ 8] = fView[ 8] * fProj[ 0] + fView[ 9] * fProj[ 4] + fView[10] * fProj[ 8] + fView[11] * fProj[12];
    fClip[ 9] = fView[ 8] * fProj[ 1] + fView[ 9] * fProj[ 5] + fView[10] * fProj[ 9] + fView[11] * fProj[13];
    fClip[10] = fView[ 8] * fProj[ 2] + fView[ 9] * fProj[ 6] + fView[10] * fProj[10] + fView[11] * fProj[14];
    fClip[11] = fView[ 8] * fProj[ 3] + fView[ 9] * fProj[ 7] + fView[10] * fProj[11] + fView[11] * fProj[15];

    fClip[12] = fView[12] * fProj[ 0] + fView[13] * fProj[ 4] + fView[14] * fProj[ 8] + fView[15] * fProj[12];
    fClip[13] = fView[12] * fProj[ 1] + fView[13] * fProj[ 5] + fView[14] * fProj[ 9] + fView[15] * fProj[13];
    fClip[14] = fView[12] * fProj[ 2] + fView[13] * fProj[ 6] + fView[14] * fProj[10] + fView[15] * fProj[14];
    fClip[15] = fView[12] * fProj[ 3] + fView[13] * fProj[ 7] + fView[14] * fProj[11] + fView[15] * fProj[15];

    // Extract the right plane
    fPlanes[0].abc[0] = fClip[ 3] - fClip[ 0];
    fPlanes[0].abc[1] = fClip[ 7] - fClip[ 4];
    fPlanes[0].abc[2] = fClip[11] - fClip[ 8];
    fPlanes[0].d = fClip[15] - fClip[12];
    fPlanes[0].normaize();

    // Extract the left plane
    fPlanes[1].abc[0] = fClip[ 3] + fClip[ 0];
    fPlanes[1].abc[1] = fClip[ 7] + fClip[ 4];
    fPlanes[1].abc[2] = fClip[11] + fClip[ 8];
    fPlanes[1].d = fClip[15] + fClip[12];
    fPlanes[1].normaize();

    // Extract the bottom plane
    fPlanes[2].abc[0] = fClip[ 3] + fClip[ 1];
    fPlanes[2].abc[1] = fClip[ 7] + fClip[ 5];
    fPlanes[2].abc[2] = fClip[11] + fClip[ 9];
    fPlanes[2].d = fClip[15] + fClip[13];
    fPlanes[2].normaize();

    // Extract the top plane
    fPlanes[3].abc[0] = fClip[ 3] - fClip[ 1];
    fPlanes[3].abc[1] = fClip[ 7] - fClip[ 5];
    fPlanes[3].abc[2] = fClip[11] - fClip[ 9];
    fPlanes[3].d = fClip[15] - fClip[13];
    fPlanes[3].normaize();

    // Extract the far plane
    fPlanes[4].abc[0] = fClip[ 3] - fClip[ 2];
    fPlanes[4].abc[1] = fClip[ 7] - fClip[ 6];
    fPlanes[4].abc[2] = fClip[11] - fClip[10];
    fPlanes[4].d = fClip[15] - fClip[14];
    fPlanes[4].normaize();

    // Extract the near plane
    fPlanes[5].abc[0] = fClip[ 3] + fClip[ 2];
    fPlanes[5].abc[1] = fClip[ 7] + fClip[ 6];
    fPlanes[5].abc[2] = fClip[11] + fClip[10];
    fPlanes[5].d = fClip[15] + fClip[14];
    fPlanes[5].normaize();
}

frustum::frustum(){}
