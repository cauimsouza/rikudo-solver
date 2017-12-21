import java.awt.*;
import java.awt.image.*;
import javax.swing.JComponent;
import javax.swing.JFrame;

// Manipulation for binary images
public class BinaryImage {

	private int width; // width of the image
	private int height; // height of the image
	private int[] raster; // raster for the image

	// Constructor that instantiates an image of a specified width and height (all pixels are black)
	public BinaryImage(int width, int height) {
		this.width = width;
		this.height = height;
		raster = new int[width * height];
		for (int i = 0; i < width * height; i++)
			raster[i] = 0xFF << 24;
	}

	// Constructor that instantiates a square image of a specified size (all pixels are black)
	public BinaryImage(int size) {
		this(size, size);
	}

	// Constructor that reads an image from a specified file (.png format)
	public BinaryImage(String filename) {
		// System.out.println("Opening image from file " + filename + " ... ");
		java.awt.Image img = Toolkit.getDefaultToolkit().getImage(filename);
		PixelGrabber pg = new PixelGrabber(img, 0, 0, -1, -1, true);
		try {
			pg.grabPixels();
		} catch (InterruptedException e) {
		}
		raster = (int[]) pg.getPixels();
		width = pg.getWidth();
		height = pg.getHeight();
		if (width == -1) {
			System.out.println("Error in opening the file " + filename);
			throw new IllegalArgumentException("Error in opening the file " + filename);
		}
	}

	// Produces a printable image from the raster
	public java.awt.Image toImage() {
		ImageProducer ip = new MemoryImageSource(width, height, raster, 0, width);
		return Toolkit.getDefaultToolkit().createImage(ip);
	}

	// Return the width of the image
	public int getWidth() {
		return this.width;
	}

	// Return the height of the image
	public int getHeight() {
		return this.height;
	}

	// Return the size of the image if it is a square, and -1 otherwise
	public int getSize() {
		return (this.width == this.height ? this.width : -1);
	}

	// Set the pixel at position (x,y) to color c
	protected void setPixel(int x, int y, int c) {
		if (x < 0 || x >= width || y < 0 || y >= height) {
			throw new IllegalArgumentException("illegal position");
		}
		raster[x + width * y] = c;
	}

	// Test whether a pixel is black or white
	public boolean isBlack(int x, int y) {
		if (x < 0 || x >= width || y < 0 || y >= height) {
			throw new IllegalArgumentException("illegal position");
		}
		return (raster[x + width * y] & 0xFFFFFF) == 0;
	}

	// Blacken the pixel at position (x,y)
	public void toBlack(int x, int y) {
		this.setPixel(x, y, 0xFF000000);
	}

	// Whiten the pixel at position (x,y)
	public void toWhite(int x, int y) {
		this.setPixel(x, y, 0xFFFFFFFF);
	}

	// Fill the pixel of a squared area with black color. The area is defined by its top left corner P=(x,y) and by its size.
	public void fillAreaBlack(int x, int y, int size) {
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				toBlack(x + i, y + j);
	}

	// Fill the pixel of a squared area with white color. The area is defined by its top left corner P=(x,y) and by its size.
	public void fillAreaWhite(int x, int y, int size) {
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				toWhite(x + i, y + j);
	}

	// Test whether all pixels in a given square region are of the same color
	public boolean isConstantColor(int x, int y, int size) {
		if (x < 0 || x + size > width || y < 0 || y + size > height) {
			throw new IllegalArgumentException("illegal position");
		}
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++) {
				if (raster[x + width * y] != raster[(x + i) + width * (y + j)])
					return false;
			}
		return true;
	}
}

class ImageViewer extends JFrame {

	private static final long serialVersionUID = -7498525833438154949L;
	static int xLocation = 0;

	public ImageViewer(BinaryImage img) {
		this.setLocation(xLocation, 0);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		ImageComponent ic = new ImageComponent(img);
		add(ic);
		pack();
		setVisible(true);
		xLocation += img.getWidth();
	}

	public ImageViewer(BinaryImage img, String name) {
		this.setTitle(name);
		this.setLocation(xLocation, 0);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		ImageComponent ic = new ImageComponent(img);
		add(ic);
		pack();
		setVisible(true);
		xLocation += img.getWidth();
	}
}

class ImageComponent extends JComponent {

	private static final long serialVersionUID = -7710437354239150390L;
	private BinaryImage img;

	public ImageComponent(BinaryImage img) {
		this.img = img;
		setPreferredSize(new Dimension(img.getWidth(), img.getWidth()));
	}

	public void paint(Graphics g) {
		g.drawImage(img.toImage(), 0, 0, this);
	}
}
