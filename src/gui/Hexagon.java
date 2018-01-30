import java.awt.Polygon;

public class Hexagon extends Polygon {
	private static final long serialVersionUID = -7498525833438154940L;
	
	public int size;
	public int xcenter;
	public int ycenter;
	
	public Hexagon(int xcenter, int ycenter, int size) {
		super();
		this.xcenter = xcenter;
		this.ycenter = ycenter;
		this.size = size;
		
		form_cords();
	}
	
	public Hexagon(int xcenter, int ycenter) {
		this(xcenter, ycenter, 50);
	}
	
	public Hexagon(Offset offset, int size) {
		this(offset.x, offset.y, size);
	}
	
	public Hexagon(Offset offset) {
		this(offset.x, offset.y);
	}
	
	private void form_cords() {
		for(int i = 0; i < 6; i++) {
			double angle = Math.toRadians(60 * i + 90);
			double x = xcenter + size * Math.cos(angle);
			double y = ycenter + size * Math.sin(angle);
			
			addPoint((int)x, (int)y);
		}
	}
}
