import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import com.csvreader.CsvWriter;

public class Single {

	public static void main(String[] args) throws FileNotFoundException {

		File folder = new File("C:/Users/Harsha/Desktop/enron6/ham");
		int counter1 = 0;
		for (File fileEntry : folder.listFiles()) {
			counter1++;
			String outputFile = "C:/Users/Harsha/Desktop/enron6/ham6/file"
					+ counter1 + ".csv";
			boolean alreadyExists = new File(outputFile).exists();
			try {
				CsvWriter csvOutput = new CsvWriter(new FileWriter(outputFile),
						',');
				File file = new File(fileEntry.getAbsolutePath());
				FileReader fileReader = new FileReader(file);
				BufferedReader bufferedReader = new BufferedReader(fileReader);
				String line = "";
				int counter = 0;
				csvOutput.write("From:");
				csvOutput.flush();
				csvOutput.write("To:");
				csvOutput.flush();
				csvOutput.write("Cc:");
				csvOutput.flush();
				csvOutput.write("Subject:");
				csvOutput.flush();
				csvOutput.write("Body:");
				csvOutput.endRecord();
				StringBuffer body = new StringBuffer();
				boolean fromValue1 = false;
				boolean toValue1 = false;
				boolean ccValue1 = false;
				boolean subjectValue1 = false;
				boolean fromRecord = false;
				boolean toRecord = false;
				boolean ccRecord = false;
				boolean subjectRecord = false;
				boolean body_start = false;
				String temp_line = "";
				int flag = 0;
				boolean flag1 = true;
				ArrayList<String> csvArr = new ArrayList<String>();
				ArrayList<String> csvArr1 = new ArrayList<String>();
				while (flag1) {

					if (temp_line != "") {
						line = temp_line;
						temp_line = "";
					} else if ((line = bufferedReader.readLine()) == null) {
						break;
					}

					line = line.trim().toLowerCase();
					line = line.replaceAll("[^a-zA-Z@:]+", " ");
					if (line == "")
						continue;
					if (!body_start) { // srinim

						if (line.startsWith("from:")
								|| line.startsWith("from :")) {
							fromRecord = true;
							flag++;
						} else if (flag == 0
								&& (line.startsWith("to:") || line
										.startsWith("to :"))) {
							toRecord = true;
							flag++;
						} else if (flag == 0
								&& (line.startsWith("cc:") || line
										.startsWith("cc :"))) {
							ccRecord = true;
							flag++;
						} else if (flag == 0
								&& (line.startsWith("subject:") || line
										.startsWith("subject :"))) {
							subjectRecord = true;
						}

						if (fromRecord) {
							csvOutput.endRecord();
						} else if (toRecord) {
							csvOutput.endRecord();
						} else if (ccRecord) {
							csvOutput.endRecord();
						} else if (subjectRecord) {
							csvOutput.endRecord();
						}

					} // srinim

					fromRecord = false;
					toRecord = false;
					ccRecord = false;
					subjectRecord = false;
					if (line.startsWith("from:") || line.startsWith("to:")
							|| line.startsWith("subject:")
							|| line.startsWith("cc:")
							|| line.startsWith("from :")
							|| line.startsWith("to :")
							|| line.startsWith("subject :")
							|| line.startsWith("cc :")) {

						if (!body_start) { // srinim

							if (line.contains("from :")
									|| line.contains("from:")) {
								String fromValue = "";
								fromValue = line.substring(5, line.length());
								// fromValue = fromValue.concat(",");
								fromValue1 = true;

								System.out.println("From: " + fromValue);
								csvOutput.write(fromValue);

							} else if (line.contains("to :")
									|| line.contains("to:")) {
								String toValue = line.substring(3,
										line.length());
								// toValue = toValue.concat(",");
								toValue1 = true;

								System.out.println("To: " + toValue);
								if (!fromValue1)
									csvOutput.write(" ");
								csvOutput.write(toValue);

							} else if (line.contains("cc :")
									|| line.contains("cc:")) {
								String ccValue = line.substring(3,
										line.length());
								ccValue1 = true;
								if (!(ccValue == null)) {
									System.out.println("Cc: " + ccValue);

									csvOutput.write(ccValue);

								} else {
									System.out.println("Cc: ");
									csvOutput.write(" ");

								}
							} else if (line.contains("subject :")
									|| line.contains("subject:")) {

								String subjectValue = line.substring(8,
										line.length());
								// subjectValue = subjectValue.concat(",");
								subjectValue1 = true;
								if (!fromValue1 && !toValue1 && !ccValue1) {
									csvOutput.write(" ");
									csvOutput.write(" ");
									csvOutput.write(" ");
								} else if (!fromValue1 && !toValue1) {
									csvOutput.write(" ");
								}

								System.out.println("Subject: " + subjectValue);
								csvOutput.write(subjectValue);

							}
							continue;
						}

						else {
							body_start = false;
							temp_line = line;
							if (!(csvArr1.isEmpty())) {
								String fullString = "";
								for (String str : csvArr1) {
									System.out.println("Array : " + str);
									fullString = fullString.concat(str);
								}
								System.out.println("Full String : "
										+ fullString);
								System.out
										.println("\nFull line : " + temp_line);
								csvOutput.write(fullString);
								csvOutput.endRecord();
								csvArr1.clear();
							}
							csvOutput.flush();
						}

					}

					else {

						if (!body_start)
							body_start = true;

						String bodyValue = "";
						bodyValue = line.substring(0, line.length());
						csvArr1.add(bodyValue);

					}

				}
				System.out.println("***********************************"
						+ csvArr);

				body_start = false;

				if (!(csvArr1.isEmpty())) {
					String fullString = "";
					for (String str : csvArr1) {
						System.out.println("Array : " + str);
						fullString = fullString.concat(str);
					}
					System.out.println("Full String : " + fullString);
					System.out.println("\nFull line : " + temp_line);
					csvOutput.write(fullString);
					csvOutput.endRecord();
					csvArr1.clear();
				}

				csvOutput.flush();

			} catch (IOException e) {

				e.printStackTrace();
			}

		}
	}
}