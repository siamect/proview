int	xnav_crr_signal(
			XNavBrow *brow,
			char	*filename,
			char	*signalname,
			flow_tNode parent_node);
int	xnav_crr_object(
			XNavBrow *brow,
			char	*filename,
			char	*objectname,
			flow_tNode parent_node);
int	xnav_crr_code(
			XNavBrow *brow,
			char	*filename,
			char	*str,
			int	brief,
			int	func,
			int	case_sensitive);

