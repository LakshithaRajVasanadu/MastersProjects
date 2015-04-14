import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Writer;
import java.util.ArrayList;
import java.util.List;

public class BreakingText {
	static int i = 0;

	public static void main(String[] args) throws IOException {
		BreakingText bt = new BreakingText();
		List<String> li = new ArrayList<String>();
		try {

			FileInputStream fs = new FileInputStream(
					"/HADOOP/hdfs/user/bigdata04/2.txt");
			DataInputStream ds = new DataInputStream(fs);
			BufferedReader br = new BufferedReader(new InputStreamReader(ds));
			String line = "";
			
			while ((line = br.readLine()) != null) {

				line = line.trim().toLowerCase();

				if (line.length() > 0) {

					if (line.contains("****")) {

						File file = new File(
								"C:/Users/Harsha/Desktop/NewText/Fileb" + bt.i
										+ ".txt");
						Writer fileWriter = new FileWriter(file);
						BufferedWriter bufferedWriter = new BufferedWriter(
								fileWriter);
						int counter = 0;

						for (String lines : li) {
							System.out.println("loop:" + lines);
							if (counter == 0) {
								bufferedWriter.write(lines);
								bufferedWriter.flush();
								counter++;
							} else {
								bufferedWriter.newLine();
								bufferedWriter.write(lines);
								bufferedWriter.flush();

							}
							bt.i++;

						}
						li.clear();

					} else {
						if (line.contains("PST and NSF format by ZL Technologies,")
								|| line.contains("Attribution 3.0 United States License <http://creativecommons.org/licenses/by/3.0/us/>")
								|| line.contains("please cite to ZL Technologies, Inc. (http://www.zlti.com).")
								|| line.contains("!!!Unknown database.")
								|| line.contains("Alias: dbCaps")
								|| line.contains("Cannot perform this operation on a closed database")) {
							continue;
						} else {

							line = line.replaceAll("[^a-zA-Z@.]+", " ");
							li.add(line);
						}
					}
					// i++;
				}
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
