import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class testingPixels {

	
	
	
	public static void main(String[] args) {

		BufferedImage img = null;
		try {
		    img = ImageIO.read(new File("../../input.png"));
		} catch (IOException e) {
		    e.printStackTrace();
		}
		
		
		for(int i=0, y=0; ; )
		{
			
		}

	}

}
