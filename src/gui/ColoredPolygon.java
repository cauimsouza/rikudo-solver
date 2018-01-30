import java.awt.Color;
import java.awt.Polygon;

public class ColoredPolygon {
	Polygon polygon;
	Color insideColor;
	Color boundaryColor;

	public ColoredPolygon(Polygon polygon, Color insideColor, Color boundaryColor) {
		this.polygon = polygon;
		this.insideColor = insideColor;
		this.boundaryColor = boundaryColor;
	}
	
	public ColoredPolygon(int[] xcoords, int[] ycoords, Color insideColor, Color boundaryColor) {
		assert(xcoords.length == ycoords.length);
		polygon = new Polygon(xcoords, ycoords, xcoords.length);
		this.insideColor = insideColor;
		this.boundaryColor = boundaryColor;
	}
}
