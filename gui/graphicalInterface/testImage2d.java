import java.awt.Color;

public class testImage2d {
	public static void main(String[] args) {
		/*Image2d img = new Image2d(1024);
		//img.addPolygon(new int[] {50,100,150,200}, new int[] {50,150,200,100}, Color.BLUE, Color.RED);
		Axial axial = new Axial(2, 2);
		Axial axial2 = new Axial(3, 2);
		Axial axial3 = new Axial(1, 2);
		img.addHexagon(axial, Color.BLUE, Color.RED);
		
		img.addHexagon(axial2, Color.RED, Color.RED);
		img.addHexagon(axial3, Color.BLACK, Color.RED);
		
		Offset offset = new Offset(100, 100);
		img.addHexagon(offset, Color.BLUE, Color.RED);
		
		img.addSegment(50, 200, 150, 100, 10, Color.GREEN);
		new Image2dViewer(img);	*/
		new RikudoGUI().draw();
	}
}
