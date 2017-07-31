package versioning

import groovy.transform.CompileStatic
import groovy.transform.ToString
import groovy.transform.TypeChecked
import org.joda.time.format.DateTimeFormat
import org.joda.time.DateTime

@CompileStatic @TypeChecked
@ToString(includeNames = true)
class MetamodVersionInfo {
	Integer majorVersion
	Integer minorVersion
	Integer maintenanceVersion
	String suffix

	boolean localChanges
	DateTime commitDate
	String commitSHA
	String commitURL
	Integer commitCount

	String asMavenVersion(boolean extra = true, String separator = ".") {
		StringBuilder sb = new StringBuilder()
		sb.append(majorVersion).append(separator).append(minorVersion);
		if (maintenanceVersion != null) {
			sb.append(separator).append(maintenanceVersion);
		}

		if (commitCount != null) {
			sb.append(separator).append(commitCount)
		}

		if (extra && suffix) {
			sb.append('-' + suffix)
		}

		// do mark for this build like a modified version
		if (extra && localChanges) {
			sb.append('+m');
		}

		return sb.toString()
	}
	String asCommitDate(String pattern = null) {
		if (pattern == null) {
			pattern = "MMM  d yyyy";
			if (commitDate.getDayOfMonth() >= 10) {
				pattern = "MMM d yyyy";
			}
		}

		return DateTimeFormat.forPattern(pattern).withLocale(Locale.ENGLISH).print(commitDate);
	}
	String asCommitTime() {
		return DateTimeFormat.forPattern('HH:mm:ss').withLocale(Locale.ENGLISH).print(commitDate);
	}
}
