package jpwr.app;

import java.io.UnsupportedEncodingException;

import android.app.Activity;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.RectF;
import android.graphics.Typeface;

public class PlowDraw implements PlowDrawIfc {
	public static final int FONT_NOMAL = 0;
	public static final int FONT_NORMAL_BOLD = 1;
	public static final int FONT_SANSSERIF = 2;
	public static final int FONT_SANSSERIF_BOLD = 3;
	public static final int FONT_DROIDSANS = 4;
	public static final int FONT_DROIDSANS_BOLD = 5;
	
	Activity activity;
	Canvas canvas;
	Paint paint;
	Typeface[] fonts = new Typeface[6];
	int canvasWidth;
	int canvasHeight;
	static float[] inv = {	-1f, 0f, 0f, 1f, 1f,
		0f,-1f, 0f, 1f, 1f,
		0f, 0f,-1f, 1f, 1f,
		0f, 0f, 0f, 1f, 0f};
	
	public PlowDraw(Activity activity) {
		this.activity = activity;
		paint = new Paint();

		fonts[0] = Typeface.create(Typeface.DEFAULT, Typeface.NORMAL);
		fonts[1] = Typeface.create(Typeface.DEFAULT, Typeface.BOLD);
		fonts[2] = Typeface.create(Typeface.SANS_SERIF, Typeface.NORMAL);
		fonts[3] = Typeface.create(Typeface.SANS_SERIF, Typeface.BOLD);
		fonts[4] = Typeface.createFromAsset(activity.getAssets(), "fonts/DroidSans.ttf");
		fonts[5] = Typeface.createFromAsset(activity.getAssets(), "fonts/DroidSans-Bold.ttf");
	}

	public void setCanvas(Canvas canvas) {
		this.canvas = canvas;
		canvasWidth = canvas.getWidth();
		canvasHeight = canvas.getHeight();
	}
	public int getCanvasWidth() {
		return canvasWidth;
	}
	public int getCanvasHeight() {
		return canvasHeight;
	}
	
	@Override
	public void rect(boolean border, int color, float x1, float y1, float x2, float y2) {

		if ( border) {
			paint.setColor(getColor(color));
			paint.setStyle(Paint.Style.STROKE);
			paint.setStrokeWidth(1);
			canvas.drawRect(x1, y1, x2, y2, paint);			
		}
		else {
			paint.setColor(getColor(color));
			paint.setStyle(Paint.Style.FILL);
			canvas.drawRect(x1, y1, x2, y2, paint);

		}
	}

	@Override
	public void arc(boolean border, int color, float x1, float y1, float x2, float y2, float angel1, float angel2) {

		if ( border) {
			paint.setColor(getColor(color));
			paint.setStyle(Paint.Style.STROKE);
			paint.setStrokeWidth(1);
			canvas.drawArc(new RectF(x1, y1, x2, y2), 360-angel1, -angel2, false, paint);			
		}
		else {
			paint.setColor(getColor(color));
			paint.setStyle(Paint.Style.FILL);
			canvas.drawArc(new RectF(x1, y1, x2, y2), 360-angel1, -angel2, true, paint);			
		}
	}

	@Override
	public void line(boolean border, int color, float x1, float y1, float x2, float y2) {

		paint.setColor(getColor(color));
		paint.setStyle(Paint.Style.STROKE);
		paint.setStrokeWidth(1);
		canvas.drawLine(x1, y1, x2, y2, paint);			
	}

	@Override
	public void pixmap(PlowPixmapData pixmapData, int idx, boolean invert, float x, float y) {
    	if ( invert) {
    		ColorMatrix cm = new ColorMatrix(inv);
    		paint.setColorFilter(new ColorMatrixColorFilter(cm));  		
     	}
    	else 
    		paint.setColor(Color.BLACK);

		canvas.drawBitmap(pixmapData.bitmap[idx], x, y, paint);

		if ( invert)
			paint.setColorFilter(null);
	}
	
	@Override
	public void drawText( String text, int textColor, int textSize, int font, float x, float y) {
		paint.setTextSize(textSize);
    	paint.setTypeface(fonts[font]);
		paint.setColor(getColor(textColor));		
		paint.setStyle(Paint.Style.FILL);
		canvas.drawText( text, x, y, paint);
	}

	@Override
	public float measureText( String text, int textSize, int font) {
    	paint.setTextSize(textSize);
    	paint.setTypeface(null);

    	return paint.measureText(text);
	}

	public void arrow(int color, float x1, float y1, float x2, float y2, float x3, float y3) {
		Path path = new Path();
		
		paint.setColor(getColor(color));		
		paint.setStyle(Paint.Style.FILL);

		path.moveTo(x1, y1);
		path.lineTo(x2, y2);
		path.lineTo(x3, y3);
		path.lineTo(x1, y1);
		canvas.drawPath(path, paint);
	}
	
	public int getColor(int color) {
		switch ( color) {
		case Plow.COLOR_BLACK:
			return Color.BLACK;
		case Plow.COLOR_RED:
			return Color.RED;
		case Plow.COLOR_WHITE:
			return Color.WHITE;
		case Plow.COLOR_GRAY:
			return Color.GRAY;
		case Plow.COLOR_DARKGRAY:
			return Color.DKGRAY;
		case Plow.COLOR_LIGHTGRAY:
			return Color.LTGRAY;
		case Plow.COLOR_YELLOW:
			return Color.YELLOW;
		case Plow.COLOR_GREEN:
			return Color.GREEN;			
		case Plow.COLOR_LIGHTBLUE:
			return Color.rgb(196, 200, 240);			
		case Plow.COLOR_BLUE:
			return Color.rgb(195,226,255);			
		case Plow.COLOR_VIOLET:
			return Color.rgb(218,198,255);			
		default:
			return Color.BLACK;
		}
	}
}
