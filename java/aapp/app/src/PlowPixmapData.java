package jpwr.app;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;

public class PlowPixmapData {
	Bitmap[] bitmap = new Bitmap[8];
	
	public PlowPixmapData(PlowCmn cmn, int id) {
		Bitmap bm = BitmapFactory.decodeResource( cmn.getResources(), id);
		int width = bm.getWidth();
		int height = bm.getHeight();
		for ( int i = 0; i < 8; i++) {
			Matrix matrix = new Matrix();
			matrix.postScale(0.25f + 0.5f * i, 0.25f + 0.5f * i);
			bitmap[i] = Bitmap.createBitmap(bm, 0, 0, width, height, matrix, false);
		}
	}

	public PlowPixmapData(PlowCmn cmn, int[] id) {
		for ( int i = 0; i < 8; i++) {
			Bitmap bm = BitmapFactory.decodeResource( cmn.getResources(), id[i]);
			int width = bm.getWidth();
			int height = bm.getHeight();
			Matrix matrix = new Matrix();
			bitmap[i] = Bitmap.createBitmap(bm, 0, 0, width, height, null, false);
		}
	}

}
