import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;
import java.util.StringTokenizer;

import com.independentsoft.msg.Message;
import com.independentsoft.pst.Folder;
import com.independentsoft.pst.Item;
import com.independentsoft.pst.PstFile;

public class Convertor {
	public static void main(String[] args) {

		File folderName = new File("C:/Users/Harsha/Desktop/PST");
		try {

			for (File fileEntry : folderName.listFiles()) {
				List<String> wordList = new ArrayList<String>();
				String newPath = "";
				String path = fileEntry.getAbsolutePath();
				System.out.println(path);
				StringTokenizer st = new StringTokenizer(path, "\\");
				while (st.hasMoreTokens()) {
					wordList.add(st.nextToken());
				}
				for (int i = 0; i <= wordList.size() - 1; i++) {
					if (i == wordList.size() - 1) {
						newPath = newPath.concat(wordList.get(i) + '"');
					} else if (i == 0) {
						newPath = newPath.concat('"' + wordList.get(i) + "/");

					} else {
						newPath = newPath.concat(wordList.get(i) + "/");
					}
				}

				System.out.println(newPath);
				String pathOne = "C:/Users/Harsha/Desktop/PST/albert_meyers_000_1_1.pst";
				System.out.println(path);

				PstFile file = new PstFile(pathOne);

				try {
					List<Folder> allFolders = file.getRoot().getFolders(true);

					Hashtable<Long, String> parents = new Hashtable<Long, String>();

					String parentFolderPath = "/Users/lakshitha/IdeaProjects/HelloWorld/src/eml";

					File newFolder = new File(parentFolderPath);
					newFolder.mkdirs();

					parents.put(file.getRoot().getId(), parentFolderPath);

					for (int p = 0; p < allFolders.size(); p++) {
						Folder currentFolder = allFolders.get(p);

						parentFolderPath = parents.get(currentFolder
								.getParentId());

						newFolder = new File(parentFolderPath + "\\"
								+ currentFolder.getDisplayName());
						newFolder.mkdirs();

						parents.put(currentFolder.getId(), parentFolderPath
								+ "\\" + currentFolder.getDisplayName());
					}

					for (int j = 0; j < allFolders.size(); j++) {
						for (int s = 0; s < allFolders.get(j)
								.getChildrenCount(); s += 100) {
							List<Item> items = allFolders.get(j).getItems(s,
									s + 100);

							for (int k = 0; k < items.size(); k++) {
								Message message = items.get(k).getMessageFile();
								System.out
										.println(" Body:" + message.getBody());
								System.out.println("-> "
										+ message.getSenderEmailAddress());
								System.out.println("-> "
										+ message.getReceivedByEmailAddress());
								System.out
										.println("-> " + message.getSubject());
							}
						}
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
				// catch (MessagingException e)
				// {
				// e.printStackTrace();
				// }
				finally {
					if (file != null) {
						file.close();
					}
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	private static String getFileName(String subject) {
		if (subject == null || subject.length() == 0) {
			String fileName = "NoSubject";
			return fileName;
		} else {
			String fileName = "";

			for (int i = 0; i < subject.length(); i++) {
				if (subject.charAt(i) > 31 && subject.charAt(i) < 127) {
					fileName += subject.charAt(i);
				}
			}

			fileName = fileName.replace("\\", "_");
			fileName = fileName.replace("/", "_");
			fileName = fileName.replace(":", "_");
			fileName = fileName.replace("*", "_");
			fileName = fileName.replace("?", "_");
			fileName = fileName.replace("\"", "_");
			fileName = fileName.replace("<", "_");
			fileName = fileName.replace(">", "_");
			fileName = fileName.replace("|", "_");

			return fileName;
		}
	}

}
