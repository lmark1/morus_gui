/*
 * MonitorWorker.h
 *
 *  Created on: Mar 12, 2018
 *      Author: lmark
 */

#ifndef MONITORWORKER_H_
#define MONITORWORKER_H_

#include <QObject>

/**
 * This class implements a QObject worker for use with QThreads.
 * Worker will spin a passive node which collects information about other
 * nodes in the given interfaces.
 */
class MonitorWorker : public QObject {
	Q_OBJECT

	public:
		explicit MonitorWorker(QObject *parent = 0);
		virtual ~MonitorWorker();

		/**
		 * Initialize the worker. Worker cannot start unless
		 * this method is called.
		 *
		 * @param ifaceName - Node interface name
		 * @param nodeId - Node ID
		 */
		void initializeWorker(std::string& ifaceName, int nodeId);

		/**
		 * Stops the worker.
		 */
		void stopWorker();

	public slots:

		/**
		 * Whole workload is done in this method.
		 * Creating CAN node, spinning CAN node etc.
		 */
		void process();

	signals:

		/**
		 * Emit this signal when worker is done.
		 */
		void finished();

		/**
		 * Emit this signal when something goes wrong.
		 */
		void error(QString err);

	private:

		/**
		 * CAN interface name.
		 */
		std::string ifaceName_;

		/**
		 * New CAN node ID.
		 */
		int nodeID_ = -1;

		/**
		 *	Flag indicating worker started processing.
		 */
		bool working_ = false;
};

#endif /* MONITORWORKER_H_ */
